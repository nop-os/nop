#ifndef __NOP_MEM_H__
#define __NOP_MEM_H__

#include <nop/type.h>

typedef struct mem_node_t mem_node_t;

struct mem_node_t {
  uint32_t size: 31;
  uint8_t  free: 1;
} __attribute__((packed));

extern void *mem_heap;
extern size_t mem_size;
extern size_t mem_used;

void mem_init(size_t size);
void mem_defrag(void);

void *mem_alloc(size_t size);
void *mem_realloc(void *ptr, size_t new_size);
void  mem_free(void *ptr);

#endif