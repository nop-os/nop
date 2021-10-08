#include <nop/send.h>
#include <nop/type.h>
#include <nop/dbg.h>
#include <string.h>

__attribute__((__section__(".entry"), __used__))
int nex_start(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  int term = nop_find("TERM");
  if (!term) return 0;
  
  char *intro = "SHEL.NEX r01, by segfaultdev\n\n";
  nop_send(term, "WRIT", (uint32_t)(intro), strlen(intro), 0);
  
  for (;;) {
    char *prompt = "nop> ";
    
    nop_send(term, "WRIT", (uint32_t)(prompt), strlen(prompt), 0);
    nop_send(term, "ECHO", 1, 0, 0);
    
    char buffer[256];
    int length = 0;
    
    for (;;) {
      char c;
      
      while (!nop_send(term, "READ", (uint32_t)(&c), 1, 0));
      if (c == '\n') break;
      
      if (c == '\b') {
        length--;
      } else if (length < 256) {
        buffer[length++] = c;
      }
    }
    
    nop_send(term, "ECHO", 0, 0, 0);
  }

  return 0;
}
