[org 0x7C00]
[bits 16]

%define TINYBOOT_DAP     0x7000 ; Extended INT 0x13 DAP
%define TINYBOOT_DRIVE   0x7010 ; Current INT 0x13 drive
%define TINYBOOT_LBA     0x7012 ; LBA address of partition
%define TINYBOOT_BUFFER  0x3000 ; FS and VESA temp. buffer
%define TINYBOOT_CLUSTER 0x2000 ; Cluster buffer
%define TINYBOOT_CONFIG  0x1000 ; Config file buffer

tinyboot_bpb:
  cli
  jmp tinyboot_stage_1

times 0x0080 - ($ - $$) db 0xCC

tinyboot_stage_1:
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov bx, 0x7000
  mov ss, bx
  mov sp, ax
  cld
  mov cx, 0x0010
  mov di, TINYBOOT_DAP
  rep stosb
  sti
.check_drv:
  test dl, 0x80
  jz drv_error
.check_lba:
  push dx
  mov ah, 0x41
  mov bx, 0x55AA
  int 0x13
  jc ext_error
  cmp bx, 0xAA55
  jne ext_error
  pop dx
.setup_dap:
  mov si, TINYBOOT_DAP
  mov byte [si + 0x00], 0x10
  mov byte [si + 0x02], 0x03
  mov byte [si + 0x05], 0x7E

  mov di, mbr_table
  mov cl, 0x04
.check_mbr_atr:
  mov eax, [di + 0x08]
  test dword [di], 0x80
  jnz .load_stage_2
  add di, 0x0010
  loop .check_mbr_atr

  xor eax, eax
.load_stage_2:
  mov [TINYBOOT_DRIVE], dl
  mov [TINYBOOT_LBA], eax
  inc eax
  mov dword [si + 0x08], eax
  mov ah, 0x42
  int 0x13
  jc rea_error

  call .wait_a20_1
  mov al, 0xAD
  out 0x64, al
  call .wait_a20_1
  mov al, 0xD0
  out 0x64, al
.wait_a20_2:
  in al, 0x64
  test al, 0x01
  jz .wait_a20_2
  in al, 0x60
  push ax
  call .wait_a20_1
  mov al, 0xD1
  out 0x64, al
  call .wait_a20_1
  pop ax
  or al, 0x02
  out 0x60, al
  call .wait_a20_1
  mov al, 0xAE
  out 0x64, al
  
  jmp 0x0000:0x7E00

.wait_a20_1:
  in al, 0x64
  test al, 0x02
  jnz .wait_a20_1
  ret

drv_error:
  mov si, drv_error_str
  jmp error
ext_error:
  mov si, ext_error_str
  jmp error
rea_error:
  mov si, rea_error_str
  jmp error
brk_error:
  mov si, brk_error_str
error:
  mov ah, 0x0E
  xor bh, bh
.str_loop:
  lodsb
  test al, al
  jz .str_end
  int 0x10
  jmp .str_loop
.str_end:
  jmp $

drv_error_str:
  db "tinyboot: Floppy boot unsupported", 0x00
ext_error_str:
  db "tinyboot: LBA unsupported", 0x00
rea_error_str:
  db "tinyboot: Read error", 0x00
brk_error_str:
  db "tinyboot: Breakpoint", 0x00

times 0x01B8 - ($ - $$) db 0xCC

mbr_uuid:
  dd 0x00000000
mbr_reserved:
  dw 0x0000

mbr_table:
  times 0x40 db 0x00

mbr_signature:
  dw 0xAA55

%include "stage_2.inc"
