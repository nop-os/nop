config_token_next:
  cmp byte [si], ' '
  jle .space_loop
  cmp byte [si], '\'
  je .skip_slash
  inc si
  jmp config_token_next
.space_loop:
  cmp byte [si], ' '
  jg .space_end
  inc si
  jmp .space_loop
.skip_slash:
  inc si
  inc si
  jmp config_token_next
.space_end:
  ret

config_entry_next:
  push di
.config_loop:
  call config_token_next
  mov di, config_entry_end_str
  call func_str_cmp_short
  jne .config_loop
  call config_token_next
  pop di
  ret

config_entry_read:
  push si
  call config_token_next
  call config_token_next
.read_loop:
  call func_str_cmp_short
  je .end
  call config_token_next
  call config_token_next
  jmp .read_loop
.end:
  call config_token_next
  pop di
  ret

config_file_str:
  db "file", 0x00
config_addr_str:
  db "addr", 0x00
config_vidw_str:
  db "vidw", 0x00
config_vidh_str:
  db "vidh", 0x00
config_vidb_str:
  db "vidb", 0x00
config_entry_end_str:
  db "}", 0x00
config_list_end_str:
  db "end", 0x00
