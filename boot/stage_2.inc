tinyboot_stage_2:
  call menu_init
  
  xor ax, ax
  mov es, ax
  mov si, fat32_config_str
  mov di, TINYBOOT_CONFIG
  call fat32_read

  call menu_draw
  jmp $

%include "print.inc"
%include "funcs.inc"
%include "drive.inc"
%include "config.inc"
%include "video.inc"
%include "fat32.inc"
%include "menu.inc"
%include "boot.inc"

times 0x0800 - ($ - $$) db 0xCC
