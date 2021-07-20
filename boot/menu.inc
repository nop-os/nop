menu_init:
  mov ah, 0x01
  mov ch, 0x3F
  int 0x10
  ret

menu_draw:
  push es
  mov ax, 0xB800
  mov es, ax

  mov ah, 0x70
  mov cx, 0x0050
  xor di, di
  rep stosw
  mov cl, 0x50
  mov di, 0x0F00
  rep stosw

  mov dx, 0x0001
  call goto_xy
  mov si, menu_top_str
  call print_str
  mov dx, 0x1801
  call goto_xy
  mov si, menu_btm_str
  call print_str
  pop es

.menu_loop:
  push es
  mov ax, 0xB800
  mov es, ax

  mov di, 0x00A0
  mov ah, 0x1F
  mov cx, 0x0730
  rep stosw

  mov si, TINYBOOT_CONFIG

  cmp byte [si], '#'
  jne .skip_next
  call config_token_next
.skip_next:

  xor cx, cx
  mov di, 0x0144
.entry_loop:
  ; cmp cl, [menu_entry_cnt]
  ; jge .entry_end
  cmp cl, [menu_entry_pos]
  jne .skip_curr
  pusha
  mov ax, 0x7000
  mov cx, 0x4C
  rep stosw
  popa
.skip_curr:
  add di, 0x00A0
  mov dx, 0x0203
  add dh, cl
  call goto_xy
  call print_short_str
  call config_entry_next
  inc cl
  push di
  mov di, config_list_end_str
  call func_str_cmp_short
  pop di
  jne .entry_loop
.entry_end:
  pop es
  mov [menu_entry_cnt], cl

.read_key:
  xor ax, ax
  int 0x16

  cmp ah, 0x1C
  je .menu_boot
  ; cmp ah, 0x3B
  ; je .menu_help
  cmp ah, 0x3D
  je .menu_quit
  cmp ah, 0x48
  je .menu_up
  cmp ah, 0x50
  je .menu_down
  jmp .read_key
.menu_quit:
  mov ax, 0x1000
  mov ax, ss
  mov sp, 0xf000
  mov ax, 0x5307
  mov bx, 0x0001
  mov cx, 0x0003
  int 0x15
  jmp $
.menu_up:
  cmp byte [menu_entry_pos], 0x00
  je .read_key
  dec byte [menu_entry_pos]
  jmp .menu_loop
.menu_down:
  mov cl, [menu_entry_cnt]
  test cl, cl
  jz .read_key
  dec cl
  cmp [menu_entry_pos], cl
  jge .read_key
  inc byte [menu_entry_pos]
  jmp .menu_loop
.menu_boot:
  mov si, TINYBOOT_CONFIG
  cmp byte [si], '#'
  jne .skip_token_next
  call config_token_next
.skip_token_next:
  movzx cx, byte [menu_entry_pos]
.skip_entry_loop:
  test cx, cx
  jz .skip_entry_end
  dec cx
  call config_entry_next
  jmp .skip_entry_loop
.skip_entry_end:
  xor ax, ax
  mov es, ax
  mov di, config_addr_str
  call config_entry_read
  call func_str_dec
  mov [boot_addr], ax
  mov si, di
  mov di, config_file_str
  call config_entry_read
  push di
  xor di, di
  mov es, ax
  call fat32_read
  xor ax, ax
  mov es, ax
  pop si
  mov di, config_vidw_str
  call config_entry_read
  call func_str_dec
  mov bx, ax ; Width
  mov si, di
  mov di, config_vidh_str
  call config_entry_read
  call func_str_dec
  mov cx, ax ; Height
  mov si, di
  mov di, config_vidb_str
  call config_entry_read
  call func_str_dec
  mov dx, ax ; Bits Per Pixel
  call video_set
  jmp boot_entry

boot_addr:
  dw 0x0000

menu_entry_cnt:
  db 0x00
menu_entry_pos:
  db 0x00

menu_top_str:
  db "tinyboot", 0x00
menu_btm_str:
  db "ENTER=Boot  ", 0x18, "/", 0x19, "=Move  F3=Quit", 0x00
