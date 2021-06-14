[bits 16]

section .text

extern nop_start

global nop_cs_16

global mem_map_ptr
global mem_map_cnt

; Hopefully the stack won't override it...
%define nop_map_ptr 0x0600

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

  ; Now we have to get the memory map from the BIOS,
  ; Starting by getting the first entry(and checking
  ; if available)
  xor ebx, ebx

  ; Also clear the entry count!
  mov [mem_map_cnt], ebx

  ; Set some registers...
  mov eax, 0x0000E820
  mov ecx, 0x00000014
  mov edx, 0x534D4150
  mov di, nop_map_ptr

  ; ...and run!
  int 0x15

  ; Test if E820 is available and if not jump
  ; to alternative E820(for pre-2002 PCs, not
  ; recommended)
  jc .alt_E820

  ; If it is, increase count, test some stuff
  ; and enter the loop
  inc dword [mem_map_cnt]
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
  jne .skip_di

  ; If all of them are true, skip the next part
  jmp .done_di

.skip_di:
  ; Decrease the entry count
  dec dword [mem_map_cnt]

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
  inc dword [mem_map_cnt]
  jmp .loop_E820

.end_E820:
  ; Check if the entry is free
  cmp dword [di + 0x10], 0x00000001
  jne .ignore_last

  ; Check if the entry is below the 4 GiB mark
  cmp dword [di + 0x04], 0x00000000
  jne .ignore_last

  ; If they are, update the counter
  inc dword [mem_map_cnt]

.ignore_last:
  ; Let the user know that we finished with maps
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
  ; could do is to cry in ~~an~~ a sad, infinite loop
  ; forever (typo totally discovered by segfaultdev and
  ; not aa2006 so no need to put him on the contrib
  ; section and everyone's happy yayyyyyyy, also this
  ; is going to be rewritten and this file won't be
  ; necesary so no E801 shit yayyyyyyyy)
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
  mov di, nop_map_ptr

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
  mov dword [di + 40], 0x01000000
  mov       [di + 44], eax
  mov       [di + 48], edx
  mov       [di + 52], eax
  mov dword [di + 56], 0x00000001

  ; Increase the map count to 2!
  add dword [mem_map_cnt], 0x02

  ; Now, continue from where we left it
  jmp .end_E820

; .alt_E820 will return a memory map like this:
; |    Base    |       Size       |   Type   |
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

mem_map_ptr:
  dd nop_map_ptr
mem_map_cnt:
  dd 0x00000000

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
