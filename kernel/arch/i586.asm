[bits 32]

; String operations

global i586_rep_stosb
global i586_rep_stosw
global i586_rep_stosd

global i586_rep_movsb
global i586_rep_movsw
global i586_rep_movsd

global i586_rep_insb
global i586_rep_insw
global i586_rep_insd

global i586_repe_cmpsb
global i586_repe_cmpsw
global i586_repe_cmpsd

global i586_repne_cmpsb
global i586_repne_cmpsw
global i586_repne_cmpsd

global i586_repe_scasb
global i586_repe_scasw
global i586_repe_scasd

global i586_repne_scasb
global i586_repne_scasw
global i586_repne_scasd

; I/O

global i586_inb
global i586_inw
global i586_ind

global i586_outb
global i586_outw
global i586_outd

; Other

global i586_set_cr3
global i586_clr_cr3

global i586_sti
global i586_cli

global i586_lidt

; Implementations(see i586/instr.h for more info):

i586_rep_stosb:
  pushad
  mov al,  [esp + 32 +  4]
  mov ecx, [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep stosb
  popad
  ret

i586_rep_stosw:
  pushad
  mov ax,  [esp + 32 +  4]
  mov ecx, [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep stosw
  popad
  ret

i586_rep_stosd:
  pushad
  mov eax, [esp + 32 +  4]
  mov ecx, [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep stosd
  popad
  ret

i586_rep_movsb:
  pushad
  mov ecx, [esp + 32 +  4]
  mov edi, [esp + 32 +  8]
  mov esi, [esp + 32 + 12]
  rep movsb
  popad
  ret

i586_rep_movsw:
  pushad
  mov ecx, [esp + 32 +  4]
  mov edi, [esp + 32 +  8]
  mov esi, [esp + 32 + 12]
  rep movsw
  popad
  ret

i586_rep_movsd:
  pushad
  mov ecx, [esp + 32 +  4]
  mov edi, [esp + 32 +  8]
  mov esi, [esp + 32 + 12]
  rep movsd
  popad
  ret

i586_rep_insb:
  pushad
  mov ecx, [esp + 32 +  4]
  mov dx,  [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep insb
  popad
  ret

i586_rep_insw:
  pushad
  mov ecx, [esp + 32 +  4]
  mov dx,  [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep insw
  popad
  ret

i586_rep_insd:
  pushad
  mov ecx, [esp + 32 +  4]
  mov dx,  [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep insd
  popad
  ret

i586_repe_cmpsb:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repe cmpsb
  mov eax, edi
  pop ecx
  pop esi
  pop edi
  ret

i586_repe_cmpsw:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repe cmpsw
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repe_cmpsd:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repe cmpsd
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repne_cmpsb:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repne cmpsb
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repne_cmpsw:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repne cmpsw
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repne_cmpsd:
  push edi
  push esi
  push ecx
  mov ecx, [esp + 12 +  4]
  mov edi, [esp + 12 +  8]
  mov esi, [esp + 12 + 12]
  repne cmpsd
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repe_scasb:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repe scasb
  mov eax, edi
  pop ecx
  pop edi
  ret

i586_repe_scasw:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repe scasw
  mov eax, edi
  pop ecx
  pop edi
  ret

i586_repe_scasd:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repe scasd
  mov eax, edi
  pop ecx
  pop edi
  ret

i586_repne_scasb:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repne scasb
  mov eax, edi
  pop ecx
  pop edi
  ret

i586_repne_scasw:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repne scasw
  mov eax, edi
  pop ecx
  pop edi
  ret

i586_repne_scasd:
  push edi
  push ecx
  mov eax, [esp + 8 +  4]
  mov ecx, [esp + 8 +  8]
  mov edi, [esp + 8 + 12]
  repne scasd
  mov eax, edi
  pop ecx
  pop edi
  ret

i586_inb:
  push edx
  mov edx, [esp + 4 + 4]
  in al, dx
  pop edx
  ret

i586_inw:
  push edx
  mov edx, [esp + 4 + 4]
  in ax, dx
  pop edx
  ret

i586_ind:
  push edx
  mov edx, [esp + 4 + 4]
  in eax, dx
  pop edx
  ret

i586_outb:
  pushad
  mov eax, [esp + 32 + 4]
  mov edx, [esp + 32 + 8]
  out dx, al
  popad
  ret

i586_outw:
  pushad
  mov eax, [esp + 32 + 4]
  mov edx, [esp + 32 + 8]
  out dx, ax
  popad
  ret

i586_outd:
  pushad
  mov eax, [esp + 32 + 4]
  mov edx, [esp + 32 + 8]
  out dx, eax
  popad
  ret

i586_set_cr3:
  pushad
  mov edi, [esp + 32 + 4]
  mov cr3, edi
  mov eax, cr0
  or eax, 0x80000000
  mov cr0, eax
  popad
  ret

i586_clr_cr3:
  pushad
  mov edi, [esp + 32 + 4]
  mov eax, cr0
  and eax, 0x7FFFFFFF
  mov cr0, eax
  popad
  ret

i586_sti:
  sti
  ret

i586_cli:
  cli
  ret

i586_lidt:
  pushad
  mov edi, [esp + 32 + 4]
  mov [.idt], edi
  lidt [.idt_ptr]
  popad
  ret
.idt_ptr:
  dw 0x0800
.idt:
  dd 0x00000000
