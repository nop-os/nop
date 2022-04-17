global prog_handle
extern prog_return

prog_handle:
  push eax
  mov eax, [prog_return]
  call eax
  jmp $
