#ifndef __NOP_PAGE_H__
#define __NOP_PAGE_H__

#include <boot/tinyboot.h>
#include <nop/type.h>

#define PAGE_SIZE        4096
#define PAGE_TABLE_COUNT 1024
#define PAGE_COUNT       (4294967296 / PAGE_SIZE)
#define PAGE_ENTRY_COUNT (PAGE_COUNT / PAGE_TABLE_COUNT)
#define PAGE_DIR_SIZE    (PAGE_TABLE_COUNT << 2)
#define PAGE_TABLE_SIZE  (PAGE_ENTRY_COUNT << 2)
#define PAGE_BITMAP_SIZE (PAGE_COUNT >> 3)

extern uint8_t *page_bitmap;
extern size_t page_mem;

void page_init(tb_mem_t *mem_table);

void *page_alloc(size_t count);
void  page_free(void *block, size_t count);

#endif
