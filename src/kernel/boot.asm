[bits 16]

section .text

extern nop_start
global nop_cs_16

; Hopefully the stack won't override it...
%define nop_mem_cnt 0x06FC
%define nop_mem_map 0x0700

; tinyboot leaves the current int 13h drive ID here
%define nop_drive_id 0x7010

nop_cs_16:
  ; tinynoot sets CS to 0x0800 and NOT 0x0000 as it
  ; should be!
  jmp 0x0000:nop_start_16

nop_start_16:
  ; Clean this, as tinyboot leaves "random" values here
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  ; Good enough until we jump into protected mode and
  ; have a full memory map
  mov sp, 0x7000

  jmp .alt_E820

  ; Now we have to get the memory map from the BIOS,
  ; Starting by getting the first entry(and checking
  ; if available)
  xor ebx, ebx

  ; Also clear the entry count!
  mov [nop_mem_cnt], ebx

  ; Set some registers...
  mov eax, 0x0000E820
  mov ecx, 0x00000014
  mov edx, 0x534D4150
  mov di, nop_mem_map

  ; ...and run!
  int 0x15

  ; Test if E820 is available and if not jump
  ; to alternative E820(for pre-2002 PCs, not
  ; recommended)
  jc .alt_E820

  ; If it is, increase count, test some stuff
  ; and enter the loop
  inc dword [nop_mem_cnt]
  test ebx, ebx
  je .end_E820

.loop_E820:
  ; Increase buffer pointer...
  add di, 0x0014

  ; ...and do practically the same as before
  mov eax, 0x0000E820
  mov edx, 0x534D4150
  int 0x15

  ; Test if it is the last entry
  jc .end_E820
  test ebx, ebx
  je .end_E820

  ; Repeat loop
  inc dword [nop_mem_cnt]
  jmp .loop_E820

.end_E820:
  ; Let the user know that we reached the final part
  xor ax, ax
  mov bx, ax
  mov ax, 0xB800
  mov es, ax
  mov word [es:bx], 0x1F21

  ; Ok, now that we did all of the preparation stuff, let's
  ; start with the protected mode code
  cli
  lgdt [nop_gdt_ptr]
  mov eax, cr0
  or al, 0x01
  mov cr0, eax

  ; Now let's make the magic jump:
  jmp 0x0008:nop_start_32

.alt_E820:
  ; This is the alternative implementation of E820, using
  ; E801 instead. This will not give any memory holes,
  ; other than the ISA one
  xor ecx, ecx
  xor edx, edx
  mov ax, 0xE801
  int 0x15

  ; If E801 is not available, the only other thing we
  ; could do is to cry in pain forever, while an infinite
  ; loop runs in this computer :(
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
  shl ecx, 0x10
  shl edx, 0x16

  ; ...and prepare to fill the memory map
  xor eax, eax
  mov di, nop_mem_map

  ; First entry: Real mode memory, free(although the OS
  ; should only use it in real mode)
  mov       [di +  0], eax
  mov       [di +  4], eax
  mov dword [di +  8], 0x00100000
  mov       [di + 12], eax
  mov dword [di + 16], 0x00000001

  ; Second entry: All the memory below the 15 MiB memory
  ; hole any any, for ISA bus, free
  mov dword [di + 20], 0x00100000
  mov       [di + 24], eax
  mov       [di + 28], ecx
  mov       [di + 32], eax
  mov dword [di + 36], 0x00000001

  ; Make some magic math
  add ecx, 0x00100000
  mov ebx, 0x01000000
  sub ebx, ecx

  ; Third entry: The ISA memory hole, could be of size 0
  ; if none, reserved
  mov       [di + 40], ecx
  mov       [di + 44], eax
  mov       [di + 48], ebx
  mov       [di + 52], eax
  mov dword [di + 56], 0x00000002

  ; Fourth entry: All of the memory after that nasty ISA
  ; memory hole, free
  mov dword [di + 60], 0x01000000
  mov       [di + 64], eax
  mov       [di + 68], edx
  mov       [di + 72], eax
  mov dword [di + 76], 0x00000001

  ; Make some MORE magic math
  add edx, 0x01000000
  mov ebx, 0x00000000
  sub ebx, edx

  ; The rest of the address bus, reserved
  mov       [di + 80], edx
  mov       [di + 84], eax
  mov       [di + 88], ebx
  mov       [di + 92], eax
  mov dword [di + 96], 0x00000002

  ; Now, continue from where we left it
  jmp .end_E820

; .alt_E820 will return a memory map like this:
; |    Base    |       Size       |   Type   |
; | 0x00000000 |    0x00100000    | Reserved |
; | 0x00100000 | ---------------- |   Free   |
; | ---------- | UP TO 0x00FFFFFF | Reserved |
; | 0x01000000 | ---------------- |   Free   |
; | ---------- | UP TO 0xFFFFFFFF | Reserved |

; Protected mode from now on
[bits 32]

nop_start_32:
  ; Setup the segment registers to use
  ; the newly-loaded GDT...
  mov ax, 0x0010
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax

  ; ...and just after that, set the stack
  mov esp, 0x7000

  ; Finally, jump to our precious kernel
  call nop_start

  ; In case it returns, enter an infinite loop
  jmp $

; Wow, a GDT table!
nop_gdt:
  times 0x08 db 0x00
nop_gdt_code:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10011010 ; Access byte
  db 0b11001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)
nop_gdt_data:
  dw 0xFFFF     ; Limit(bits 0-15)
  dw 0x0000     ; Base(bits 0-15)
  db 0x00       ; Base(bits 16-23)
  db 0b10010010 ; Access byte
  db 0b11001111 ; Flags & limit(bits 16-19)
  db 0x00       ; Base(bits 24-31)

; Even better, a GDT table pointer!
nop_gdt_ptr:
  dw (nop_gdt_ptr - nop_gdt)
  dd nop_gdt

times 0x1000 - ($ - $$) db 0x00
