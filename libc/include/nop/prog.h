#ifndef __NOP_PROG_H__
#define __NOP_PROG_H__

#include <stdint.h>

typedef struct call_t call_t;

struct call_t {
  char name[32];
  int prog;
  
  // function handler
  uint32_t (*func)(uint32_t arg_1, uint32_t arg_2, uint32_t arg_3, uint32_t arg_4, uint32_t arg_5);
};

int  prog_load(const char *path, const char **argv, const char **envp, call_t *call_array, int call_count);
int  prog_kill(int id);
int  prog_wait(int id);
void prog_skip(void);
int  prog_alloc(size_t count);

#endif
