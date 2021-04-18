#ifndef __NOP_MEM_H__
#define __NOP_MEM_H__

#include <stdint.h>

#define MEM_TO_BIT(idx) (((idx) + (1 << 12) - 1) >> (12))

#define MEM_SET_BIT(ptr, idx, bit) ({(ptr)[(idx) >> 3] = ((ptr)[(idx) >> 3] & ~(1 << (7 - ((idx) & 7)))) | ((bit) << (7 - ((idx) & 7)));})
#define MEM_GET_BIT(ptr, idx)      (((ptr)[(idx) >> 3] >> (7 - ((idx & 7)))) & 1)

#define MEM_BIT_SET(ptr, idx)   (ptr[(idx) >> 3] |= (1 << ((idx) & 7)))
#define MEM_BIT_CLEAR(ptr, idx) (ptr[(idx) >> 3] &= ~(1 << ((idx) & 7)))
#define MEM_BIT_TEST(ptr, idx)  ((ptr[(idx) >> 3] >> ((idx) & 7)) & 1)

typedef struct mem_entry_t mem_entry_t;

struct mem_entry_t {
  uint32_t base, padding_1;
  uint32_t size, padding_2;

  uint32_t type;
};

extern uint8_t *mem_bitmap_ptr;
extern uint32_t mem_bitmap_len;

// In boot.asm
extern mem_entry_t *mem_map_ptr;
extern uint32_t mem_map_cnt;

void mem_init(void);

#endif
