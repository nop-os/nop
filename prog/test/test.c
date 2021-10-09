#include <nop/send.h>
#include <nop/type.h>
#include <stdio.h>

__attribute__((__section__(".entry"), __used__))
int nex_start(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  if (type != nop_type("INIT")) return 0;
  
  int term = nop_find("TERM");
  if (!term) return 0;
  
  stdio_init(term, term);
  printf("Hello, world!\n");

  return 0;
}
