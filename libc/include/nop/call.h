#include <stdint.h>

typedef struct call_t call_t;

struct call_t {
  char name[32];
  int prog;
  
  // function handler
  uint32_t (*func)(uint32_t arg_1, uint32_t arg_2, uint32_t arg_3, uint32_t arg_4, uint32_t arg_5);
};

extern uint32_t nop_call(const char *name, uint32_t arg_1, uint32_t arg_2, uint32_t arg_3, uint32_t arg_4, uint32_t arg_5);

size_t call_read(void *dest, const void *src, size_t size);
size_t call_write(void *dest, const void *src, size_t size);
int    call_getid(void);
