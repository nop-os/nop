#ifndef __NOP_ALLOC_H__
#define __NOP_ALLOC_H__

#include <nop/type.h>

void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

void free(void *ptr);

#endif
