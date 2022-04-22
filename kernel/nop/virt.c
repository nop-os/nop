#include <arch/i586.h>
#include <nop/alloc.h>
#include <nop/term.h>
#include <nop/virt.h>
#include <nop/page.h>
#include <nop/type.h>
#include <string.h>

uint32_t *virt_table = NULL;

void virt_init(void) {
  virt_table = virt_alloc();
  
  if (!virt_table) return;
  
  virt_map(virt_table, VIRT_NOP_KERN, VIRT_NOP_KERN, VIRT_WRITE, VIRT_NOP_SIZE);
  virt_load(virt_table);
  
  term_infof("initialized paging\n");
}

uint32_t *virt_alloc(void) {
  uint32_t *table = page_alloc(PAGE_DIR_SIZE >> 12, 0);
  memset(table, 0, PAGE_DIR_SIZE);
  
  return table;
}

void virt_free(uint32_t *table) {
  for (int i = 0; i < PAGE_TABLE_COUNT; i++) {
    uint32_t *sub_table = (void *)(table[i] & 0xFFFFF000);
    if (sub_table) page_free(sub_table, PAGE_TABLE_SIZE >> 12);
  }
  
  page_free(table, PAGE_DIR_SIZE >> 12);
}

void virt_load(uint32_t *table) {
  i586_set_cr3(table);
}

void *virt_phys(uint32_t *table, void *virt_addr) {
  uint32_t virt_page = (uint32_t)(virt_addr) >> 12;
  uint32_t offset = (uint32_t)(virt_addr) & 0x00000FFF;
  
  uint32_t index = virt_page >> 10;
  uint32_t *sub_table = (void *)(table[index] & 0xFFFFF000);
  
  if (!sub_table) return NULL;
  uint32_t real_page = sub_table[virt_page & 0x03FF] >> 12;
  
  return (void *)((real_page << 12) | offset);
}

size_t virt_cont(uint32_t *table, void *virt_addr) {
  void *phys_addr = virt_phys(table, virt_addr);
  virt_addr += 0x1000;
  
  size_t count = 1;
  
  while (virt_phys(table, virt_addr) == phys_addr + 0x1000) {
    phys_addr = virt_phys(table, virt_addr);
    virt_addr += 0x1000;
    
    count++;
  }
  
  return count;
}

void virt_memcpy_to_virt(uint32_t *table, void *dest, const void *src, size_t size) {
  size_t left = size;
  
  if ((uint32_t)(dest) & 0x0FFF) {
    size_t read = 0x1000 - ((uint32_t)(dest) & 0x0FFF);
    if (read > left) read = left;
    
    memcpy(virt_phys(table, (void *)(dest)), src, read);
    
    src += read;
    dest += read;
    
    left -= read;
  }
  
  while (left) {
    size_t read = virt_cont(table, (void *)(dest)) << 12;
    if (read > left) read = left;
    
    memcpy(virt_phys(table, (void *)(dest)), src, read);
    
    src += read;
    dest += read;
    
    left -= read;
  }
}

void virt_memcpy_to_phys(uint32_t *table, void *dest, const void *src, size_t size) {
  size_t left = size;
  
  if ((uint32_t)(src) & 0x0FFF) {
    size_t read = 0x1000 - ((uint32_t)(src) & 0x0FFF);
    if (read > left) read = left;
    
    memcpy(dest, virt_phys(table, (void *)(src)), read);
    
    src += read;
    dest += read;
    
    left -= read;
  }
  
  while (left) {
    size_t read = virt_cont(table, (void *)(src)) << 12;
    if (read > left) read = left;
    
    memcpy(dest, virt_phys(table, (void *)(src)), read);
    
    src += read;
    dest += read;
    
    left -= read;
  }
}

size_t virt_strlen(uint32_t *table, const char *str) {
  size_t size = 0;
  while (*((const char *)(virt_phys(table, (void *)(str) + size)))) size++;
  
  return size;
}

void virt_strncpy_to_virt(uint32_t *table, char *dest, const char *src, size_t size) {
  size_t str_size = strlen(src) + 1;
  if (str_size > size) str_size = size;
  
  virt_memcpy_to_virt(table, dest, src, str_size);
}

void virt_strncpy_to_phys(uint32_t *table, char *dest, const char *src, size_t size) {
  size_t str_size = virt_strlen(table, src) + 1;
  if (str_size > size) str_size = size;
  
  virt_memcpy_to_phys(table, dest, src, str_size);
}

char *virt_strdup(uint32_t *table, const char *str) {
  size_t str_size = virt_strlen(table, str) + 1;
  
  char *new_str = malloc(str_size);
  virt_memcpy_to_phys(table, new_str, str, str_size);
  
  return new_str;
}

void virt_map(uint32_t *table, void *phys_addr, void *virt_addr, uint32_t flags, size_t count) {
  uint32_t page = (uint32_t)(virt_addr) >> 12;
  
  for (uint32_t i = page; i < page + count; i++) {
    uint32_t index = i >> 10;
    uint32_t *sub_table = (void *)(table[index] & 0xFFFFF000);
    
    if (!sub_table) {
      sub_table = page_alloc(PAGE_TABLE_SIZE >> 12, 0);
      memset(sub_table, 0, PAGE_TABLE_SIZE);
      
      table[index] = (uint32_t)(sub_table) | VIRT_USER | VIRT_WRITE | VIRT_PRESENT;
    }
    
    sub_table[i & 0x03FF] = (((uint32_t)(phys_addr) & 0xFFFFF000) + ((i - page) << 12)) | flags | VIRT_PRESENT;
  }
}

void virt_unmap(uint32_t *table, void *virt_addr, size_t count) {
  uint32_t page = (uint32_t)(virt_addr) >> 12;
  
  for (uint32_t i = page; i < page + count; i++) {
    uint32_t index = i >> 10;
    uint32_t *sub_table = (void *)(table[index] & 0xFFFFF000);
    
    if (!sub_table) {
      continue;
    }
    
    sub_table[i & 0x03FF] = 0x00000000;
  }
}
