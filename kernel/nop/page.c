#include <boot/tinyboot.h>
#include <arch/i586.h>
#include <nop/page.h>
#include <nop/type.h>
#include <string.h>

uint8_t *page_bitmap = NULL;
size_t page_mem = 0;

void page_init(tb_mem_t *mem_table) {
  const size_t bitmap_size = PAGE_BITMAP_SIZE * sizeof(uint8_t);

  for (int i = 0; i < mem_table->count; i++) {
    if (mem_table->table[i].addr & 0x0FFF) {
      uint64_t skip = 0x1000 - (mem_table->table[i].addr & 0x0FFF);

      mem_table->table[i].addr += skip;
      mem_table->table[i].size -= skip;
    }

    if (mem_table->table[i].size & 0x0FFF) {
      uint64_t skip = mem_table->table[i].size & 0x0FFF;

      mem_table->table[i].size -= skip;
    }

    if (mem_table->table[i].size > bitmap_size) {
      page_bitmap = (uint8_t *)(mem_table->table[i].addr);
      mem_table->table[i].addr += bitmap_size;
      mem_table->table[i].size -= bitmap_size;
      break;
    }
  }

  if (!page_bitmap) return;

  memset(page_bitmap, 0xFF, bitmap_size);

  for (int i = 0; i < mem_table->count; i++) {
    page_free((void *)(mem_table->table[i].addr), mem_table->table[i].size / PAGE_SIZE);
    page_mem += mem_table->table[i].size;
  }
}

void *page_alloc(size_t count) {
  uint32_t page = 0, free = 0;
  int found = 0;

  for (uint32_t i = 0; i < PAGE_COUNT; i++) {
    int used = page_bitmap[i >> 3] & (1 << (7 - (page & 7)));

    if (used) {
      page = i + 1, free = 0;
    } else {
      free++;

      if (free >= count) {
        found = 1;
        break;
      }
    }
  }

  if (!found) return NULL;

  for (uint32_t i = page; i < page + count; i++) {
    page_bitmap[i >> 3] |= 1 << (7 - (i & 7));
  }

  page_mem -= (count * PAGE_SIZE);

  return (void *)(page * PAGE_SIZE);
}

void page_free(void *block, size_t count) {
  uint32_t page = (uint32_t)(block) / PAGE_SIZE;

  for (uint32_t i = page; i < page + count; i++) {
    page_bitmap[i >> 3] &= ~(1 << (7 - (i & 7)));
  }

  page_mem += (count * PAGE_SIZE);
}