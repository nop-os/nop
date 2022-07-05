[bits 32]

; string operations

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

global i586_std
global i586_cld

; I/O

global i586_inb
global i586_inw
global i586_ind

global i586_outb
global i586_outw
global i586_outd

global i586_wait

; context switching

global i586_switch
global i586_handle

global i586_return

; other

global i586_set_cr3
global i586_clr_cr3

global i586_sti
global i586_cli

global i586_loop

global i586_lidt

global i586_int16

; Implementations(see arch/i586.h for more info):

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

i586_std:
  std
  ret

i586_cld:
  cld
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

i586_wait:
  push edx
  mov dx, 0x0080
  out dx, al
  pop edx
  ret

i586_switch:
  mov eax, [esp + 4]
  mov edi, [esp + 8]
  mov cr3, edi
  mov esp, eax
  mov al, 0x20
  mov dx, 0x0020
  out dx, al
  popad
  sti
  iret

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

i586_loop:
  jmp $
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
  
i586_int16:
  mov [.tmp_ebx], ebx
  mov [.tmp_ecx], ecx
  mov [.tmp_edx], edx
  mov [.tmp_esi], esi
  mov [.tmp_edi], edi
  mov [.tmp_ebp], ebp
  mov [.tmp_esp], esp
  mov eax, [esp + 4]
  mov [.int_call + 1], al
  mov esi, [esp + 8]
  mov edi, 0x00007000
  mov ecx, 0x00000020
  rep movsb
  mov esi, .real_idt
  mov edi, 0x00007032
  mov ecx, 0x00000006
  rep movsb
  mov eax, .p_mode_32
  mov [.real_addr], eax
  mov esi, .real_code
  mov edi, 0x00007300
  mov ecx, 0x00000007
  rep movsb
  mov eax, .r_mode_16
  mov [0x702E], ax
  sgdt [0x00007020]
  sidt [0x00007026]
  jmp 0x0018:.p_mode_16
  [bits 16]
.p_mode_16:
  mov ax, 0x0020
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov esp, [0x700C]
  lidt [0x7032]
  mov eax, cr0 
  and eax, 0x7FFFFFFE
  mov cr0, eax
  push word 0x1000
  push word [0x702E]
  retf
.r_mode_16:
  xor ax, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  mov [0x702C], ax
  mov edi, [0x7000]
  mov esi, [0x7004]
  mov ebp, [0x7008]
  mov esp, [0x700C]
  mov ebx, [0x7010]
  mov edx, [0x7014]
  mov ecx, [0x7018]
  mov eax, [0x701C]
  sti
  clc
.int_call:
  int 0x00
  jnc .no_error
  inc word [0x702C]
.no_error:
  cli
  mov [0x7000], edi
  mov [0x7004], esi
  mov [0x7008], ebp
  mov [0x700C], esp
  mov [0x7010], ebx
  mov [0x7014], edx
  mov [0x7018], ecx
  mov [0x701C], eax
  lgdt [0x7020]
  mov eax, cr0
  or eax, 0x80000001
  mov cr0, eax
  push word 0x0008
  push word 0x7300
  retf
  [bits 32]
.p_mode_32:
  mov ax, 0x0010
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax
  movzx eax, word [0x0000702C]
  mov esp, [.tmp_esp]
  mov esi, 0x00007000
  mov edi, [esp + 8]
  mov ecx, 0x00000020
  rep movsb
  mov ebx, [.tmp_ebx]
  mov ecx, [.tmp_ecx]
  mov edx, [.tmp_edx]
  mov esi, [.tmp_esi]
  mov edi, [.tmp_edi]
  mov ebp, [.tmp_ebp]
  ; lidt [0x7026]
  ret
.real_code:
  db 0xBF
.real_addr:
  dd 0x00000000
  dw 0xE7FF
.real_idt:
  dw 0x03FF
  dd 0x00000000
.tmp_ebx:
  dd 0x00000000
.tmp_ecx:
  dd 0x00000000
.tmp_edx:
  dd 0x00000000
.tmp_esi:
  dd 0x00000000
.tmp_edi:
  dd 0x00000000
.tmp_ebp:
  dd 0x00000000
.tmp_esp:
  dd 0x00000000
