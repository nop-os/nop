%define BOOT_MAP_PTR 0xF000

boot_entry:
  xor ax, ax
  mov ds, ax
  mov es, ax

  xor ebx, ebx
  xor esi, esi

  ; Set some registers...
  mov eax, 0x0000E820
  mov ecx, 0x00000014
  mov edx, 0x534D4150
  mov di, BOOT_MAP_PTR

  ; ...and run!
  int 0x15

  ; Test if E820 is available and if not jump
  ; to alternative E820(for pre-2002 PCs, not
  ; recommended)
  jc .alt_E820

  ; If it is, increase count, test some stuff
  ; and enter the loop
  inc esi
  test ebx, ebx
  je .end_E820

.loop_E820:
  ; Check if the entry is above the 1 MiB mark
  cmp dword [di + 0x00], 0x00100000
  jl .skip_di

  ; Check if the entry is below the 4 GiB mark
  cmp dword [di + 0x04], 0x00000000
  jne .skip_di

  ; Check if the entry is free
  cmp dword [di + 0x10], 0x00000001
  je .done_di

.skip_di:
  ; Decrease the entry count
  dec esi

  ; Decrease buffer pointer to make the next
  ; addition not do anything
  sub di, 0x0014

.done_di:
  ; Increase buffer pointer
  add di, 0x0014

  ; And do practically the same as before
  mov eax, 0x0000E820
  mov edx, 0x534D4150
  int 0x15

  ; Test if it is the last entry
  jc .end_E820
  test ebx, ebx
  je .end_E820

  ; Repeat loop
  inc esi
  jmp .loop_E820

.end_E820:
  ; Check if the entry is free
  cmp dword [di + 0x10], 0x00000001
  jne .ignore_last

  ; Check if the entry is below the 4 GiB mark
  cmp dword [di + 0x04], 0x00000000
  jne .ignore_last

  ; If they are, update the counter
  inc esi

.ignore_last:
  mov dword [mem_table.table_ptr], BOOT_MAP_PTR
  mov       [mem_table.table_cnt], si

  cli
  lgdt [boot_gdt_ptr]
  mov eax, cr0
  or al, 0x01
  mov cr0, eax

  jmp 0x0008:start_32

.alt_E820:
  ; This is the alternative implementation of E820, using
  ; E801 instead. This will not give any memory holes,
  ; other than the ISA one
  xor ecx, ecx
  xor edx, edx
  mov ax, 0xE801
  int 0x15

  ; If E801 is not available, the only other thing we
  ; could do is to cry in an sad, infinite loop forever
  jc $

  ; Not again, please...
  test ah, 0x80
  jnz $

  ; Either AX.BX or CX.DX could be zero, so force it to
  ; use CX.DX only
  test cx, cx
  jnz .alt_load_map
  mov cx, ax
  mov dx, bx

.alt_load_map:
  ; Finally, fill a fake E820 memory map with the data we
  ; got

  ; First, convert the values returned by E801 to bytes...
  sub cx, 0x0400
  shl ecx, 0x0A
  shl edx, 0x10

  ; ...and prepare to fill the memory map
  xor eax, eax
  mov di, BOOT_MAP_PTR

  ; First entry: All the memory below the 15 MiB memory
  ; hole any any, for ISA bus, free
  mov dword [di +  0], 0x00100000
  mov       [di +  4], eax
  mov       [di +  8], ecx
  mov       [di + 12], eax
  mov dword [di + 16], 0x00000001

  ; Make some magic math
  add ecx, 0x00100000
  mov ebx, 0x01000000
  sub ebx, ecx

  ; Second entry: All of the memory after that nasty ISA
  ; memory hole, free
  mov dword [di + 20], 0x01000000
  mov       [di + 24], eax
  mov       [di + 28], edx
  mov       [di + 22], eax
  mov dword [di + 26], 0x00000001

  ; Increase the map count to 2!
  mov si, 0x0002

  ; Now, continue from where we left it
  jmp .end_E820

[bits 32]

start_32:
  mov ax, 0x0010
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov esp, 0x00080000

  push dword video_table
  push dword mem_table
  
  xor ebp, ebp
  
  movzx edi, word [boot_addr]
  shl edi, 4
  call edi

  jmp $

; Wow, a GDT table!
boot_gdt:
; Even better, a GDT table pointer(in the null entry)!
boot_gdt_ptr:
  dw (boot_gdt_end - boot_gdt)
  dd boot_gdt
  dw 0x0000
boot_gdt_code_32:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10011010 ; Access byte
  db 0b11001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)
boot_gdt_data_32:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10010010 ; Access byte
  db 0b11001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)
boot_gdt_code_16:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10011010 ; Access byte
  db 0b00001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)
boot_gdt_data_16:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10010010 ; Access byte
  db 0b00001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)
boot_gdt_end:

mem_table:
.table_ptr:
  dd 0x00000000
.table_cnt:
  dw 0x0000
