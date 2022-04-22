#ifndef __NOP_CALL_H__
#define __NOP_CALL_H__

#include <arch/i586.h>
#include <nop/type.h>

// default address for kernel syscall stack
#define CALL_STACK_ADDR (void *)(0x00007000)

// default address for terminal stack
#define CALL_STACK_TERM (void *)(0x00005000)

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

extern int call_flag;

void call_init(void);
int  call_find(const char *name, int prog, call_t *call);

void call_kernel(void *func, uint32_t value);

void call_handle(i586_regs_t *regs);

extern uint32_t call_switch(uint32_t eax, const void *edi);

// all the kernel syscalls:

int    $file_open(const char *path);
int    $file_create(const char *path);
int    $file_delete(const char *path);
int    $file_close(int id, int save);
size_t $file_write(int id, void *buffer, size_t size);
size_t $file_read(int id, void *buffer, size_t size);
size_t $file_seek(int id, off_t offset, int type);
int    $file_resize(int id, size_t size);

size_t $term_write(const char *buffer, size_t size);
size_t $term_read(char *buffer, size_t size);

int   $prog_load(const char *path, const char **argv, const char **envp, call_t *call_array, int call_count);
int   $prog_kill(int id);
int   $prog_wait(int id);
void  $prog_skip(void);
void *$prog_alloc(size_t count);

#endif
