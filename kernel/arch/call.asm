global call_switch

call_switch:
  mov eax, [esp + 4]
  mov ecx, [esp + 8]
  mov esi, cr3
  mov cr3, ecx
  mov dword [eax + 4], .return
  mov [eax + 28], esi
  mov [eax + 32], esp
  mov esp, eax
  ret
.return:
  add esp, 0x14
  mov edx, eax
  pop ecx
  pop eax
  mov cr3, ecx
  mov esp, eax
  mov eax, edx
  ret
