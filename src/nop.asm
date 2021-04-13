[bits 32]

nop_start_32:
  ; Yay, protected mode!

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

  ; INFITINEEEEEEEE
  jmp $
