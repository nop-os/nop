#include <nop/alloc.h>
#include <nop/call.h>
#include <nop/prog.h>
#include <stdint.h>
#include <string.h>

uint32_t *nop_heap = (void *)(0x01010000); // 64 KiB after the start of memory space, leaving space for the stack :D
size_t nop_heap_size = 0;

void *nop_alloc(size_t size) {
  size_t offset = 0;
  
  while (offset < nop_heap_size) {
    size_t block_size = nop_heap[offset] & 0x7FFFFFFF;
    
    if (nop_heap[offset] >> 31) {
      if (block_size == ((size + 3) >> 2)) {
        nop_heap[offset] &= 0x7FFFFFFF;
        
        return nop_heap + (offset + 1);
      } else if (block_size > ((size + 3) >> 2)) {
        size_t left = block_size - ((size + 3) >> 2);
        size_t new_offset = offset + (((size + 3) >> 2) + 1);
        
        nop_heap[offset] = ((size + 3) >> 2);
        nop_heap[new_offset] = 0x80000000 | (left - 1);
        
        return nop_heap + (offset + 1);
      }
    }
    
    offset += block_size + 1;
  }
  
  size_t step = NOP_HEAP_STEP >> 12;
  if (((size + 4 + 0x0FFF) >> 12) > step) step = (size + 4 + 0x0FFF) >> 12;
  
  if (!prog_alloc(step)) return NULL;
  
  nop_heap[nop_heap_size] = 0x80000000 | ((step << 10) - 1);
  nop_heap_size += step << 10;
  
  return nop_alloc(size);
}

void nop_free(void *ptr) {
  if (!ptr) return;
  if ((uint32_t)(ptr) & 3) return;
  
  uint32_t *header = ptr - 4;
  *header |= 0x80000000;
  
  // TODO: defragmentate memory!
}

void *nop_realloc(void *ptr, size_t size) {
  if ((uint32_t)(ptr) & 3) ptr = NULL;
  if (ptr == NULL) return nop_alloc(size);
  
  // TODO: do proper realloc if possible
  void *new_ptr = nop_alloc(size);
  
  uint32_t *header = ptr - 4;
  size_t block_size = (*header & 0x7FFFFFFF) * 4;
  
  memcpy(new_ptr, ptr, block_size < size ? block_size : size);
  
  nop_free(ptr);
  return new_ptr;
}
