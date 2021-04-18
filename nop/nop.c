#include <nop/device.h>
#include <nop/debug.h>
#include <nop/mem.h>
#include <string.h>

void nop_start(void) {
  dbg_clrscr();
  mem_init();
  device_init();

  dbg_printf("|   OFFSET   |    SIZE    | TYPE |\n");
  dbg_printf("|------------|------------|------|\n");

  for (int i = 0; i < mem_map_cnt; i++)
    dbg_printf("| 0x%08x | 0x%08x | %s |\n", mem_map_ptr[i].base, mem_map_ptr[i].size, mem_map_ptr[i].type == 1 ? "FREE" : "USED");

  dbg_printf("\n0x%08x, 0x%08x\n\n", mem_bitmap_ptr, mem_bitmap_len);

  for (uint32_t i = 0; i < mem_bitmap_len; i++) {
    dbg_printf("%02x  ", mem_bitmap_ptr[i]);
  }
}
