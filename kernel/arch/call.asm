global call_switch

call_switch:
  mov eax, [esp + 4]
  mov edi, [esp + 8]
  mov esi, cr3
  mov cr3, edi
  mov dword [eax + 4], .return
  mov [eax + 24], esi
  mov [eax + 28], esp
  mov esp, eax
  sti
  ret
.return:
  add esp, 0x10
  mov edx, eax
  pop edi
  pop eax
  mov cr3, edi
  mov esp, eax
  mov eax, edx
  ret
