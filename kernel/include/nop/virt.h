#ifndef __NOP_VIRT_H__
#define __NOP_VIRT_H__

#include <nop/type.h>

#define VIRT_USER    (1 << 2)
#define VIRT_WRITE   (1 << 1)
#define VIRT_PRESENT (1 << 0)

#define VIRT_NOP_KERN (void *)(0x00000000)
#define VIRT_NOP_DATA (void *)(0x01000000)
#define VIRT_NOP_CODE (void *)(0xF8000000)

#define VIRT_NOP_SIZE (0x00100000 - ((uint32_t)(VIRT_NOP_KERN) >> 12))

extern uint32_t *virt_table;

void virt_init(void);

uint32_t *virt_alloc(void);
void      virt_free(uint32_t *table);
void      virt_load(uint32_t *table);

void *virt_phys(uint32_t *table, void *virt_addr);

void virt_map(uint32_t *table, void *phys_addr, void *virt_addr, uint32_t flags, size_t count);
void virt_unmap(uint32_t *table, void *virt_addr, size_t count);

#endif
