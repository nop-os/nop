#include <arch/i586.h>
#include <nop/virt.h>
#include <nop/page.h>
#include <nop/type.h>

uint32_t *virt_table = NULL;

void virt_init(void) {
  virt_table = virt_alloc();

  if (!virt_table) return;

  virt_map(virt_table, VIRT_NOP_ADDR, VIRT_NOP_ADDR, VIRT_WRITE, VIRT_NOP_SIZE);
  virt_load(virt_table);
}

uint32_t *virt_alloc(void) {
  size_t size = PAGE_DIR_SIZE + (PAGE_TABLE_SIZE * PAGE_TABLE_COUNT);
  uint32_t *table = page_alloc(size / PAGE_SIZE);

  if (!table) return NULL;

  virt_unmap(table, 0, PAGE_COUNT);

  for (uint32_t i = 0; i < PAGE_TABLE_COUNT; i++) {
    table[i] = ((uint32_t)(table) + PAGE_DIR_SIZE + (i << 12)) | VIRT_USER | VIRT_WRITE | VIRT_PRESENT;
  }

  return table;
}

void virt_free(uint32_t *table) {
  size_t size = PAGE_DIR_SIZE + (PAGE_TABLE_SIZE * PAGE_TABLE_COUNT);
  page_free(table, size / PAGE_SIZE);
}

void virt_load(uint32_t *table) {
  i586_set_cr3(table);
}

void virt_map(uint32_t *table, void *phys_addr, void *virt_addr, uint32_t flags, size_t count) {
  uint32_t page = (uint32_t)(virt_addr) >> 12;

  for (uint32_t i = page; i < page + count; i++) {
    table[PAGE_TABLE_COUNT + i] = (((uint32_t)(phys_addr) & 0xFFFFF000) + (i << 12)) | flags | VIRT_PRESENT;
  }
}

void virt_unmap(uint32_t *table, void *virt_addr, size_t count) {
  uint32_t page = (uint32_t)(virt_addr) >> 12;

  for (uint32_t i = page; i < page + count; i++) {
    table[PAGE_TABLE_COUNT + i] = 0x00000000;
  }
}
