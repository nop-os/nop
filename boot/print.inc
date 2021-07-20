print_chr:
  pusha
  mov ah, 0x0E
  xor bh, bh
  cmp al, 0x0A
  je .newline
.print:
  int 0x10
  popa
  ret
.newline:
  mov al, 0x0A
  int 0x10
  mov al, 0x0D
  jmp .print

print_str:
  pusha
.str_loop:
  lodsb
  test al, al
  jz .str_end
  call print_chr
  jmp .str_loop
.str_end:
  popa
  ret

print_short_str:
  pusha
.str_loop:
  lodsb
  cmp al, ' '
  jle .str_end
  cmp al, '\'
  je .skip_slash
  call print_chr
  jmp .str_loop
.skip_slash:
  lodsb
  call print_chr
  jmp .str_loop
.str_end:
  popa
  ret

goto_xy:
  pusha
  xor bx, bx
  mov ah, 0x02
  int 0x10
  popa
  ret
