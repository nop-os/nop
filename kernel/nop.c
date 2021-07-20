#include <boot/tinyboot.h>

__attribute__((__section__(".entry"), __used__))
void nop(tb_mem_t *mem_table, tb_vid_t *vid_table) {
  uint32_t *fb = (uint32_t *)(vid_table->buffer);

  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      fb[j + i * (vid_table->pitch >> 2)] = 0x00FF7F00;
    }
  }

  for (;;);
}
