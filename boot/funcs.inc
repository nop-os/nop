func_str_cmp:
  pusha
.str_loop:
  cmp byte [si], 0x00
  je .str_end
  cmpsb
  je .str_loop
  dec si
  dec di
.str_end:
  cmpsb
  popa
  ret

func_str_cmp_short:
  pusha
  push es
  mov ax, ds
  mov es, ax
.str_loop:
  cmp byte [si], ' '
  jle .str_end
  cmp byte [si], '\'
  je .skip_slash
  cmpsb
  je .str_loop
  dec si
  dec di
.str_end:
  cmp byte [si], ' '
  jg .cmp_end
  cmp byte [di], 0x00
  jmp .func_end
.cmp_end:
  cmpsb
.func_end:
  pop es
  popa
  ret
.skip_slash:
  inc si
  inc di
  jmp .str_loop

func_mem_cmp:
  pusha
  push si
  repe cmpsb
  pop di
  sub si, di
  cmp si, cx
  popa
  ret

func_next_line:
.char_loop:
  cmp byte [si], 0x20
  jl .char_end
  inc si
  jmp .char_loop
.char_end:
  cmp byte [si], 0x00
  je .char_ret
  inc si
.char_ret:
  ret

func_str_dec:
  push dx
  xor ax, ax
.char_loop:
  cmp byte [si], 0x30
  jl .char_end
  mov dx, 10
  mul dx
  xor dx, dx
  mov dl, [si]
  add ax, dx
  sub ax, 0x0030
  inc si
  jmp .char_loop
.char_end:
  pop dx
  ret