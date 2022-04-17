#include <nop/alloc.h>
#include <nop/term.h>
#include <nop/page.h>
#include <string.h>

typedef struct node_t node_t;

struct node_t {
  size_t  size;
  uint8_t free: 1;
};

static void   *alloc_heap = NULL;
static size_t  alloc_size = 0;

static void init(size_t size) {
  if ((alloc_heap = page_alloc((size + 0x0FFF) >> 12, 1))) {
    alloc_size = size;

    node_t *node = alloc_heap;
    node->size = alloc_size - sizeof(node_t);
    node->free = 1;
    
    term_infof("initialized kernel heap allocator, %u KiB available\n", size >> 10);
  }
}

static void defrag(void) {
  void *ptr = alloc_heap;

  while (ptr < alloc_heap + alloc_size) {
    node_t *node = ptr;
    node_t *next_node = ptr + node->size + sizeof(node_t);

    if (node->free && next_node->free) {
      node->size += next_node->size + sizeof(node_t);
    } else {
      ptr = next_node;
    }
  }
}

void *malloc(size_t size) {
  if (!alloc_heap) {
    init(page_size / 1536);
  }
  
  void *ptr = alloc_heap;
  
  while (ptr < alloc_heap + alloc_size) {
    node_t *node = ptr;

    if (node->free) {
      if (node->size == size) {
        node->free = 0;
        return ptr + sizeof(node_t);
      } else if (node->size > size + sizeof(node_t)) {
        node_t *new_node = ptr + sizeof(node_t) + size;

        new_node->size = node->size - (sizeof(node_t) + size);
        new_node->free = 1;

        node->size = size;
        node->free = 0;
        
        term_infof("allocated block of %u bytes at 0x%p\n", size, ptr + sizeof(node_t));
        return ptr + sizeof(node_t);
      }
    }

    ptr += node->size + sizeof(node_t);
  }

  return NULL;
}

void *realloc(void *ptr, size_t new_size) {
  if (!new_size) return NULL;

  void *new_ptr = malloc(new_size);

  if (ptr) {
    node_t *node = ptr - sizeof(node_t);
    size_t size = new_size < node->size ? new_size : node->size;
    
    memcpy(new_ptr, ptr, size);
    free(ptr);
  }

  return new_ptr;
}

void free(void *ptr) {
  node_t *node = ptr - sizeof(node_t);
  node->free = 1;
  
  defrag();
}
