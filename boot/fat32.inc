%define FAT32_SECTOR_SIZE  0x7C0B
%define FAT32_CLUSTER_SIZE 0x7C0D
%define FAT32_RESERVED     0x7C0E
%define FAT32_FAT_COUNT    0x7C10
%define FAT32_HIDDEN       0x7C1C
%define FAT32_FAT_SIZE     0x7C24
%define FAT32_ROOT_CLUSTER 0x7C2C

; Requires:
; - EBX: Cluster
; Returns:
; - EBX: LBA address(limited to 2 TiB)
fat32_lba:
  push eax
  push edx
  dec ebx
  dec ebx
  movzx eax, byte [FAT32_CLUSTER_SIZE]
  mul ebx
  mov ebx, eax
  movzx eax, word [FAT32_RESERVED]
  add ebx, eax
  movzx eax, byte [FAT32_FAT_COUNT]
  mul dword [FAT32_FAT_SIZE]
  shl edx, 0x10
  or eax, edx
  add ebx, eax
  pop edx
  pop eax
  ret

; Requires:
; - EBX: Cluster
; - ES:DI: Buffer
; Returns:
; - EBX: Next cluster
; - ES:DI: Next buffer
; - Carry flag: If 1, there are more clusters
fat32_cluster:
  push eax
  push ecx
  push edx
  push ebx
  call fat32_lba
  movzx cx, byte [FAT32_CLUSTER_SIZE]
  call drive_read
  jc fat32_error
  pop ebx
  push es
  push di

  ; 512 bytes = 128 entries, divide EBX by 128 to get the sector
  xor edx, edx
  mov eax, ebx
  mov ecx, 0x00000080
  div ecx
  shl edx, 0x02
  movzx ebx, word [FAT32_RESERVED]
  add ebx, eax

  ; EBX: LBA address of gud part of FAT
  ; EDX: Offset at that gud part

  xor ax, ax
  mov es, ax
  push es
  mov di, TINYBOOT_CLUSTER
  mov cx, 0x0001
  call drive_read
  jc fat32_error
  pop es

  ; Get cluster with offset

  mov bx, dx
  mov ebx, [es:di + bx]
  and ebx, 0x0FFFFFFF

  pop di
  pop es
  cmp ebx, 0x0FFFFFF7
  jl .set_carry
  clc
  jmp .skip_carry
.set_carry:
  stc
.skip_carry:
  pop edx
  pop ecx
  pop eax
  ret


; Requires:
; - DS:SI: File name
; - ES:DI: Root dir. cluster
; Returns:
; - EBX: First cluster
; - Carry flag: If 1, the file was found
fat32_find:
  movzx ax, byte [FAT32_CLUSTER_SIZE]
  mul word [FAT32_SECTOR_SIZE]
  shr ax, 0x05
.file_loop:
  test ax, ax
  jz .not_found
  mov cx, 0x000B
  call func_mem_cmp
  jge .file_found
  dec ax
  add di, 0x0020
  jmp .file_loop
.file_found:
  push word [es:di + 0x14]
  push word [es:di + 0x1A]
  pop ebx
  stc
  jmp .end_find
.not_found:
  clc
.end_find:
  ret

; Requires:
; - DS:SI: File name
; - ES:DI: Buffer
fat32_read:
  push es
  push di
  mov ebx, [FAT32_ROOT_CLUSTER]
  xor ax, ax
  mov es, ax
.find_loop:
  mov di, TINYBOOT_BUFFER
  call fat32_cluster
  push ebx
  pushf
  xor ax, ax
  mov es, ax
  mov di, TINYBOOT_BUFFER
  call fat32_find
  jc .file_found
  popf
  jnc fat32_error
  pop ebx
  jmp .find_loop
.file_found:
  add sp, 0x0006
  pop di
  pop es
.load_cluster:
  call fat32_cluster
  jc .load_cluster
  ret

fat32_error:
  mov si, fat32_error_str
  call print_str
  jmp $

fat32_config_str:
  db "CONFIG  TXT", 0x00
fat32_help_str:
  db "HELP    TXT", 0x00

fat32_error_str:
  db "tinyboot: FAT32 error", 0x0A, 0x00
