#include <nop/term.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  int mode = term_getmode();
  term_setmode(0);
  
  for (;;) {
    char chr;
    
    if (!term_read(&chr, 1)) continue;
    if (chr) printf("(%u)", chr);
    
    if (chr == 'q') break;
  }
  
  term_setmode(mode);
  return 1;
}
