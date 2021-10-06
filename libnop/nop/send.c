#include <arch/i586.h>
#include <nop/send.h>
#include <nop/type.h>
#include <string.h>

uint32_t nop_type(const char *str) {
  uint32_t type = 0;
  
  for (int i = 0; i < 4; i++) {
    type |= ((uint32_t)(str[i]) << (i << 3));
  }
  
  return type;
}

uint32_t nop_send(int id, const char *type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  return i586_send(nop_type(type), data_1, data_2, data_3, (uint32_t)(id));
}

int nop_find(const char *prog) {
  int id = 1;
  
  while (id) {
    char buffer[5];
    int next = nop_send(0, "LIST", id, (uint32_t)(buffer), 0);
    
    if (!strcmp(buffer, prog)) {
      return id;
    }
    
    id = next;
  }
  
  return 0;
}

void *nop_phys(int id, uint32_t addr) {
  if (addr < 0xE0000000 || addr >= 0xF0000000) {
    return (void *)(addr);
  }
  
  return (void *)(nop_send(0, "PHYS", id, addr, 0));
}
