#ifndef __NOP_PROG_H__
#define __NOP_PROG_H__

#include <stdint.h>

int  prog_load(const char *path, const char **argv, const char **envp, call_t *call_array, int call_count);
int  prog_kill(int id);
int  prog_wait(int id);
void prog_skip(void);
int  prog_alloc(size_t count);

#endif
