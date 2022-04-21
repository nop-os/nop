global setjmp
global longjmp

setjmp:
  mov eax, [esp + 4]
  mov [eax +  0], ebx
  mov [eax +  4], esi
  mov [eax +  8], edi
  mov [eax + 12], ebp
  mov ecx, esp
  add ecx, 8
  mov [eax + 16], ecx
  mov ecx, [esp]
  mov [eax + 20], ecx
  xor eax, eax
  ret

longjmp:
  mov edx, [esp + 4]
  mov eax, [esp + 8]
  test eax, eax
  jnz .val_ok
  inc eax
.val_ok:
  mov ebx, [edx +  0]
  mov esi, [edx +  4]
  mov edi, [edx +  8]
  mov ebp, [edx + 12]
  mov ecx, [edx + 16]
  mov esp, ecx
  mov ecx, [edx + 20]
  jmp ecx
