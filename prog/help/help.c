#include <nop/term.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  const char *entry = "intro";
  
  if (argc > 1) {
    if (argc > 2) {
      printf("help: invalid argument(s)\n");
      return 0;
    }
    
    entry = argv[1];
  }
  
  char path[256];
  strcpy(path, "0:/help/");
  
  strcat(path, entry);
  strcat(path, ".txt");
  
  FILE *file = fopen(path, "r");
  
  if (!file) {
    printf("help: cannot open '%s'\n", path);
    return 0;
  }
  
  uint32_t term_size = term_getsize();
  
  int width = (term_size >> 0) & 0xFFFF;
  int height = (term_size >> 16) & 0xFFFF;
  
  int curr_x = 0;
  int curr_y = 0;
  
  int old_mode = term_getmode();
  
  term_setmode(0);
  putstr("\x1B[2J");
  
  char chr;
  
  for (;;) {
    while (curr_y < height - 1) {
      if (!fread(&chr, 1, 1, file)) {
        chr = '\n';
      }
      
      putchar(chr);
      curr_x++;
      
      if (chr == '\n' || curr_x >= width) {
        curr_x = 0;
        curr_y++;
      }
    }
    
    printf("[help: '%s' - q=quit, w=start, enter=advance] ", entry);
    chr = getchar();
    
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
