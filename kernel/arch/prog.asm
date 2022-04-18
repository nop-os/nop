global prog_handle
extern prog_return

prog_handle:
  push eax
  call prog_return
  jmp $
