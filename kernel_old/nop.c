#include <boot/tinyboot.h>
#include <devs/fbr_tty.h>
#include <arch/i586.h>
#include <devs/tty.h>
#include <devs/ata.h>
#include <devs/fat.h>
#include <devs/fbr.h>
#include <nop/page.h>
#include <nop/virt.h>
#include <nop/mem.h>
#include <nop/dbg.h>
#include <nop/idt.h>
#include <nop/pci.h>
#include <string.h>

__attribute__((__section__(".entry"), __used__))
void nop(tb_mem_t *mem_table, tb_vid_t *vid_table) {
  page_init(mem_table);
  virt_init();
  mem_init(page_size / 512);

  idt_init();

  fbr_init_all(vid_table);
  fbr_tty_init_all();

  conn_t *tty0 = conn_connect("tty0");
  dbg_init(tty0);
  dbg_infof("welcome to nop!\n");

  pci_init();
  
  tty_init_all();
  ata_init_all();

  /*

  uint8_t buffer[512];

  conn_t *blk0 = conn_connect("blk0");
  conn_seek(blk0, 0, conn_seek_set);
  conn_read(blk0, buffer, 512);

  for (int i = 0; i < 16; i++) {
    dbg_printf("%04X: ", i * 32);

    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 16; k++) {
        dbg_printf("%02X", buffer[i * 32 + j * 16 + k]);
      }

      dbg_printf(" ");
    }

    dbg_printf("|");

    for (int j = 0; j < 32; j++) {
      if (buffer[i * 32 + j] < 32 || buffer[i * 32 + j] >= 127) {
        dbg_printf(".");
      } else {
        dbg_printf("%c", buffer[i * 32 + j]);
      }
    }

    dbg_printf("|\n");
  }

  conn_release(blk0);

  */

  fat_init_all();

  dbg_infof("mem: %d%% used\n", ((page_used >> 12) * 100) / (page_size >> 12));

  for (;;);
}
