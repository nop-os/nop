[org 0xE0000000]
[bits 32]

nex_header:
  db "NEX!" ; header signature
  db "INIT" ; program name
nex_start:
  pushad
  
  mov eax, [esp + 32 + 4]
  cmp eax, 0x4B434954 ; is TICK?
  je tick
  
  xor edi, edi ; send to SYST
  mov eax, 0x454D4954 ; TIME
  xor ebx, ebx ; the kernel can already guess the program ID sooooooo
  mov ecx, 1 ; enable ticks
  
  int 0x30 ; call the kernel!
  
  popad
  ret

tick:
  mov edi, string
  
print:
  cmp byte [edi], 0x00
  je end
  
  mov dx, 0x03F8
  mov al, [edi]
  out dx, al
  
  inc edi
  jmp print

end:
  popad
  ret

string:
  db "I'm a userspace program, and I attached an event handler to every tick(10ms) and it fuckin' works!", 0x0D, 0x0A, 0x00
