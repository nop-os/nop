#ifndef __NOP_TASK_H__
#define __NOP_TASK_H__

#include <nop/type.h>
#include <nop/virt.h>

typedef struct task_t task_t;

struct task_t {
  uint32_t *table;
  int (*start)(void);
};

int task_call(task_t task);

#endif