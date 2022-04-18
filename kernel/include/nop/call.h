#ifndef __NOP_CALL_H__
#define __NOP_CALL_H__

#include <arch/i586.h>
#include <nop/type.h>

// default address for kernel syscall stack
#define CALL_STACK_ADDR (void *)(0x00007000)

// size of stack allocated for each syscall context
#define CALL_STACK_SIZE 4096

typedef struct call_t call_t;

struct call_t {
  char name[32];
  int prog;
  
  // function handler
  uint32_t (*func)(uint32_t arg_1, uint32_t arg_2, uint32_t arg_3, uint32_t arg_4, uint32_t arg_5);
};

extern const call_t call_array[];
extern const int call_count;

void call_init(void);
int  call_find(const char *name, int prog, call_t *call);

void call_handle(i586_regs_t *regs);

extern uint32_t call_switch(uint32_t eax, const void *edi);

// all the kernel syscalls
size_t $term_write(const char *buffer, size_t size);

#endif
