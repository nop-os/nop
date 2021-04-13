; Ok, if you are wondering what the heck is
; this file: This is the nop boot header,
; to which is appended the nop kernel. This
; header is in charge of setting up all of
; the protected mode stuff(except paging),
; and a memory map. You could make it work
; with any other bootloader you want by
; changing the include statement below.
; Don't worry, there is only one :).

; Hopefully the stack won't override it
; before we copy it...
%define nop_mem_cnt 0x06FC
%define nop_mem_map 0x0700

; Let 'em do all of the magic
%include "boot/boot_tinyboot.inc"

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

nop_start_32:
