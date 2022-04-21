#ifndef __STDLIB_H__
#define __STDLIB_H__

#include <stdnoreturn.h>
#include <setjmp.h>
#include <stdint.h>

extern char **environ;

extern jmp_buf _start_buf;
extern int _start_ret;

void *malloc(size_t size);
void  free(void *ptr);

void *calloc(size_t count, size_t size);
void *realloc(void *ptr, size_t size);

int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);

const char *getenv(const char *name);

noreturn void exit(int value);

#endif
