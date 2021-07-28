#include <nop/type.h>
#include <nop/page.h>
#include <nop/dbg.h>
#include <nop/mem.h>
#include <string.h>

void *mem_heap = NULL;
size_t mem_size = 0;
size_t mem_used = 0;

void mem_init(size_t size) {
  if ((mem_heap = page_alloc((size + 0x0FFF) >> 12))) {
    mem_size = size;

    mem_node_t *node = mem_heap;
    node->size = mem_size - sizeof(mem_node_t);
    node->free = 1;
  }
}

void mem_defrag(void) {
  void *ptr = mem_heap;

  while (ptr < mem_heap + mem_size) {
    mem_node_t *node = ptr;
    mem_node_t *next_node = ptr + node->size + sizeof(mem_node_t);

    if (node->free && next_node->free) {
      node->size += next_node->size + sizeof(mem_node_t);
    } else {
      ptr = next_node;
    }
  }
}

void *mem_alloc(size_t size) {
  void *ptr = mem_heap;
  
  while (ptr < mem_heap + mem_size) {
    mem_node_t *node = ptr;

    if (node->free) {
      if (node->size == size) {
        node->free = 0;
        mem_used += size;

        return ptr + sizeof(mem_node_t);
      } else if (node->size > size + sizeof(mem_node_t)) {
        mem_node_t *new_node = ptr + sizeof(mem_node_t) + size;

        new_node->size = node->size - (sizeof(mem_node_t) + size);
        new_node->free = 1;

        node->size = size;
        node->free = 0;

        mem_used += size;

        return ptr + sizeof(mem_node_t);
      }
    }

    ptr += node->size + sizeof(mem_node_t);
  }

  return NULL;
}

void *mem_realloc(void *ptr, size_t new_size) {
  if (!new_size) return NULL;

  void *new_ptr = mem_alloc(new_size);

  if (ptr) {
    mem_node_t *node = ptr - sizeof(mem_node_t);
    size_t size = new_size < node->size ? new_size : node->size;
    
    memcpy(new_ptr, ptr, size);
    mem_free(ptr);
  }

  return new_ptr;
}

void mem_free(void *ptr) {
  mem_node_t *node = ptr - sizeof(mem_node_t);
  node->free = 1;

  mem_used -= node->size;
  mem_defrag();
}

void mem_list(void) {
  void *ptr = mem_heap;
  
  dbg_infof("mem: listing heap map\n");

  while (ptr < mem_heap + mem_size) {
    mem_node_t *node = ptr;

    dbg_infof("- addr=0x%08X, size=0x%08X, free=%d\n", ptr + sizeof(mem_node_t), node->size, node->free);
    ptr += node->size + sizeof(mem_node_t);
  }
}