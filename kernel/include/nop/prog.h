#ifndef __NOP_PROG_H__
#define __NOP_PROG_H__

#include <arch/i586.h>
#include <nop/type.h>
#include <nop/idt.h>

#define PROG_MAX 64

#define PROG_INIT 0x54494E49
#define PROG_TICK 0x4B434954
#define PROG_TRIG 0x47495254
#define PROG_DEAD 0x44414544

typedef struct prog_t prog_t;

struct prog_t {
  char name[4];
  
  void *buffer, *start;
  uint32_t size;
  
  int free, tick;
};

extern prog_t *prog_arr;
extern size_t prog_idx;
extern int prog_id;

void     prog_init(void);
int      prog_push(prog_t prog);
uint32_t prog_call(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3);

void prog_tick(i586_regs_t *regs, idt_hand_t *hand);

#endif
