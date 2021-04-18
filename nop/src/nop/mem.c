#include <nop/mem.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

uint8_t *mem_bitmap_ptr = NULL;
uint32_t mem_bitmap_len = 0;

void mem_init(void) {
  // We shouldn't rely on the memory map entries
  // being sorted by address, so find the bitmap
  // size by getting the end of the last entry
  for (uint32_t i = 0; i < mem_map_cnt; i++) {
    uint32_t end = mem_map_ptr[i].base + mem_map_ptr[i].size;

    // If the end is outside the current bitmap
    // length, then make the bitmap bigger. Note
    // that we don't need to check the type as the
    // boot header already deletes used entries
    if (end > mem_bitmap_len) mem_bitmap_len = end;
  }

  // Now we have to convert the size from real
  // bytes to bitmap ones by dividing by the
  // page size(2^12) times 8(2^3);
  mem_bitmap_len = (mem_bitmap_len + (1 << (12 + 3)) - 1) >> (12 + 3);

  // Our next step is to find an entry big
  // enough for storing the bitmap on
  for (uint32_t i = 0; i < mem_map_cnt; i++) {
    if (mem_map_ptr[i].size >= mem_bitmap_len) {
      // Yay we found one!
      mem_bitmap_ptr = (uint8_t *)(mem_map_ptr[i].base);

      // Also make the entry smaller as we
      // added the bitmap
      mem_map_ptr[i].base += mem_bitmap_len;
      mem_map_ptr[i].size -= mem_bitmap_len;

      // No need to check more entries
      break;
    }
  }

  // Set all of the bitmap as used(all bits
  // to one)
  memset(mem_bitmap_ptr, 0xFF, mem_bitmap_len);

  // The final step is to mark all of the
  // entries on the memory map as free(bits
  // to zero)
  for (uint32_t i = 0; i < mem_map_cnt; i++) {
    uint32_t start = MEM_TO_BIT(mem_map_ptr[i].base);
    uint32_t end = MEM_TO_BIT(mem_map_ptr[i].base + mem_map_ptr[i].size);

    for (; start <= end; start++) {
      MEM_BIT_CLEAR(mem_bitmap_ptr, start);
    }
  }
}
