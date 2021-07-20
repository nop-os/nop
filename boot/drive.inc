drive_read:
  add ebx, [TINYBOOT_LBA]
.load_loop:
  test cx, cx
  jz .load_end
  pushad
  mov si, TINYBOOT_DAP
  mov byte  [si + 0x00], 0x10
  mov byte  [si + 0x02], 0x01
  mov word  [si + 0x04], di
  mov word  [si + 0x06], es
  mov dword [si + 0x08], ebx
  mov ah, 0x42
  mov dl, [TINYBOOT_DRIVE]
  clc
  int 0x13
  jc .error_end
  popad
  mov ax, es
  add ax, 0x0020
  mov es, ax
  inc ebx
  dec cx
  jmp .load_loop
.error_end:
  popad
  ret
.load_end:
  sub ebx, [TINYBOOT_LBA]
  ret