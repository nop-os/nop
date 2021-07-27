#include <boot/tinyboot.h>
#include <devs/fbr_tty.h>
#include <arch/i586.h>
#include <devs/tty.h>
#include <devs/ata.h>
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

  dbg_infof("mem: %d%% used\n", ((page_used >> 12) * 100) / (page_size >> 12));
  for (;;);
}
