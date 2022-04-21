#ifndef __SETJMP_H__
#define __SETJMP_H__

#include <stdnoreturn.h>

typedef void *jmp_buf[6];

extern int setjmp(jmp_buf env);
extern noreturn void longjmp(jmp_buf env, int value);

#endif
