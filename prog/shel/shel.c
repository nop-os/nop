#include <nop/send.h>
#include <nop/type.h>
#include <nop/dbg.h>
#include <string.h>

__attribute__((__section__(".entry"), __used__))
int nex_start(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  int term = nop_find("TERM");
  if (!term) return 0;
  
  char *str = "> Hey, I'm writing this in SHEL.NEX!";
  nop_send(term, "WRIT", (uint32_t)(str), strlen(str), 0);
  
  return 0;
}
