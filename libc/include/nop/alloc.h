#ifndef __NOP_ALLOC_H__
#define __NOP_ALLOC_H__

#include <stdint.h>

#define NOP_HEAP_STEP 32768

extern uint32_t *nop_heap;
extern size_t nop_heap_size;

void *nop_alloc(size_t size);
void  nop_free(void *ptr);

void *nop_realloc(void *ptr, size_t size);

#endif
