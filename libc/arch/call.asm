global nop_call

nop_call:
  mov esi, [esp +  4]
  mov eax, [esp +  8]
  mov ebx, [esp + 12]
  mov ecx, [esp + 16]
  mov edx, [esp + 20]
  mov edi, [esp + 24]
  int 0x30
  ret
