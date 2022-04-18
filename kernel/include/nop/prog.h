#ifndef __NOP_PROG_H__
#define __NOP_PROG_H__

#include <arch/i586.h>
#include <nop/call.h>
#include <nop/type.h>

#define PROG_COUNT 256

typedef struct prog_data_t prog_data_t;
typedef struct prog_t prog_t;

struct prog_data_t {
  void *data;
  size_t size;
  
  int (*entry)(const char **argv, const char **argc);
};

struct prog_t {
  int free, done;
  
  // parent program id
  int parent;
  
  // where the program is actually loaded at
  prog_data_t data;
  
  // when it returns, done is set to 1 and this is set to the return value
  int value;
  
  // for context switching
  i586_regs_t regs;
  uint32_t *table;
  
  // the amount of allocated pages, 16 by default
  size_t page_count;
  
  // custom callset(if any)
  call_t *call_array;
  int call_count;
};

extern prog_t *prog_list;
extern int prog_id;

extern int prog_waiting;

void prog_init(void);
void prog_start(void);

int prog_load(const char *path, const char **argv, const char **envp, call_t *call_array, int call_count);
int prog_kill(int id);

int prog_alloc(int id, size_t count);

void prog_switch(i586_regs_t *regs);
void prog_return(uint32_t value);

extern void prog_handle(void);

#endif
