#include <boot/tinyboot.h>
#include <arch/i586.h>
#include <nop/page.h>
#include <nop/virt.h>
#include <nop/mem.h>

__attribute__((__section__(".entry"), __used__))
void nop(tb_mem_t *mem_table, tb_vid_t *vid_table) {
  uint8_t foo[] = {0xC0, 0xFE, 0xEB, 0xAD};
  uint8_t bar[] = {0xC0, 0xFE, 0xEB, 0xEE};

  page_init(mem_table);
  virt_init();

  mem_init(page_mem / 512);

  uint32_t *fb = vid_table->buffer;

  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 100; j++) {
      fb[j + i * (vid_table->pitch >> 2)] = 0x00FF7F00;
    }
  }
  
  for (;;);
}
