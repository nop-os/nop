#ifndef __NOP_TERM_H__
#define __NOP_TERM_H__

#include <stdint.h>

#define TERM_ECHO (1 << 0)
#define TERM_COOK (1 << 1)

size_t   term_write(const char *buffer, size_t size);
size_t   term_read(char *buffer, size_t size);
int      term_getmode(void);
int      term_setmode(int mode);
uint32_t term_getsize(void);

#endif
