; Finds and sets a VESA video mode
;   BX = Width
;   CX = Height
;   DL = BPP
; Returns:
;   EDI = Pointer to table with framebuffer, bytes per line, bits per pixel and resolution
video_set:
  mov di, TINYBOOT_BUFFER

  xor ax, ax
  mov es, ax

  mov [video_table.tmp_width], bx
  mov [video_table.tmp_height], cx
  mov [video_table.tmp_bpp], dl
  
  test dl, dl
  jz .mode_old

  mov dword [es:di], 0x32454256
  mov ax, 0x4F00
  int 0x10

  mov ax, [es:di + 16]
  mov ds, ax

  mov si, [es:di + 14]

  add di, 0x0200

.mode_loop:
  mov cx, [ds:si]

  ; cmp cx, 0xFFFF
  ; je $

  pusha
  mov ax, 0x4F01
  int 0x10
  popa

  test word [es:di], 0x0080
  jz .mode_fail

  cmp [es:di + 18], bx
  jne .mode_fail

  mov ax, [video_table.tmp_height]
  cmp [es:di + 20], ax
  jne .mode_fail

  cmp [es:di + 25], dl
  je .mode_found

.mode_fail:
  add si, 0x0002
  jmp .mode_loop

.mode_found:
  mov bx, cx
  or bh, 0x40

  mov ax, 0x4F02
  int 0x10

  mov eax, [es:di + 40]
  mov [video_table.tmp_buffer], eax
  mov ax, [es:di + 16]
  mov [video_table.tmp_pitch], ax
  
  ret
  
.mode_old:
  mov ax, bx
  int 0x10
  
  ret

video_table:
.tmp_buffer:
  dd 0x00000000
.tmp_width:
  dw 0x0000
.tmp_height:
  dw 0x0000
.tmp_pitch:
  dw 0x0000
.tmp_bpp:
  db 0x00
