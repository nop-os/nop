#include <nop/term.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 2) {
    printf("%s: invalid argument(s)\n", argv[0]);
    return 0;
  }
  
  FILE *file = fopen(argv[1], "r");
  
  if (!file) {
    printf("%s: cannot open '%s'\n", argv[0], argv[1]);
    return 0;
  }
  
  uint32_t term_size = term_getsize();
  
  int height = (term_size >> 16) & 0xFFFF;
  int curr_y = 0;
  
  int old_mode = term_getmode();
  
  term_setmode(0);
  putstr("\x1B[2J");
  
  uint8_t buffer[16];
  
  for (;;) {
    while (curr_y < height - 1) {
      int read = fread(buffer, 16, 1, file);
      
      if (read) {
        printf("%08X  ", ftell(file) - read);
        
        for (int i = 0; i < 16; i++) {
          if (i < read) printf("%02X", buffer[i]);
          else putstr("--");
          
          if (i % 8 == 7) putstr("  ");
          else putchar(' ');
        }
        
        putchar('|');
        
        for (int i = 0; i < 16; i++) {
          if (i < read) {
            if (buffer[i] >= 32 && buffer[i] < 127) {
              putchar(buffer[i]);
            } else {
              putchar('.');
            }
          } else {
            putchar(' ');
          }
        }
        
        putchar('|');
      }
      
      putchar('\n');
      curr_y++;
    }
    
    printf("[dump: '%s' - q=quit, w=start, enter=advance] ", argv[1]);
    char chr = getchar();
    
    if (chr == 'q') {
      break;
    } else if (chr == 'w') {
      putstr("\x1B[2J");
      curr_y = 0;
      
      rewind(file);
    } else {
      putstr("\x1B[2K\r");
      
      if (!feof(file)) {
        curr_y--;
      }
    }
  }
  
  term_setmode(old_mode);
  putchar('\n');
  
  fclose(file);
  return 1;
}
