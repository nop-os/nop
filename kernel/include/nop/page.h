#ifndef __NOP_PAGE_H__
#define __NOP_PAGE_H__

#include <nop/type.h>

#define PAGE_SIZE 4096

extern uint32_t *page_ptr;

void page_init(void);

void *page_alloc(size_t count);
void  page_free(void *block);

#endif
