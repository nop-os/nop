#ifndef __NOP_PROG_H__
#define __NOP_PROG_H__

#include <arch/i586.h>
#include <nop/type.h>
#include <nop/idt.h>

#define PROG_MAX 256

typedef struct prog_t prog_t;

struct prog_t {
  uint32_t *virt_table;
  
  void *buffer;
  uint32_t size;
  
  int free, tick;
};

extern prog_t *prog_arr;
extern size_t prog_idx;

void prog_init(void);
int  prog_push(prog_t prog);
int  prog_call(uint32_t type, void *data, int id);

void prog_tick(i586_regs_t *regs, idt_hand_t *hand);

#endif
