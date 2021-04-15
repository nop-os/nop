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
  rep stosb
  popad
  ret

i586_rep_stosd:
  pushad
  mov eax, [esp + 32 +  4]
  mov ecx, [esp + 32 +  8]
  mov edi, [esp + 32 + 12]
  rep stosb
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
  mov edi, [esp + 8 + 4]
  mov esi,  [esp + 8 + 8]
  repe cmpsb
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repe_cmpsw:
  push edi
  push esi
  mov edi, [esp + 8 + 4]
  mov esi,  [esp + 8 + 8]
  repe cmpsw
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repe_cmpsd:
  push edi
  push esi
  mov edi, [esp + 8 + 4]
  mov esi,  [esp + 8 + 8]
  repe cmpsd
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repne_cmpsb:
  push edi
  push esi
  mov edi, [esp + 8 + 4]
  mov esi,  [esp + 8 + 8]
  repne cmpsb
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repne_cmpsw:
  push edi
  push esi
  mov edi, [esp + 8 + 4]
  mov esi,  [esp + 8 + 8]
  repne cmpsw
  mov eax, edi
  pop esi
  pop edi
  ret

i586_repne_cmpsd:
  push edi
  push esi
  mov edi, [esp + 8 + 4]
  mov esi,  [esp + 8 + 8]
  repne cmpsd
  mov eax, edi
  pop esi
  pop edi
  ret
