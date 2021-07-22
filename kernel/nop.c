#include <boot/tinyboot.h>
#include <arch/i586.h>
#include <devs/tty.h>
#include <devs/pci.h>
#include <nop/page.h>
#include <nop/virt.h>
#include <nop/mem.h>
#include <nop/dbg.h>
#include <nop/idt.h>
#include <string.h>

__attribute__((__section__(".entry"), __used__))
void nop(tb_mem_t *mem_table, tb_vid_t *vid_table) {
  page_init(mem_table);
  virt_init();
  mem_init(page_mem / 512);
  tty_init_all();

  conn_t *tty0 = conn_connect("tty0");
  dbg_init(tty0);
  dbg_infof("welcome to nop!\n");

  dbg_infof("memory table:\n");

  for (int i = 0; i < mem_table->count; i++) {
    dbg_infof("- entry %d: addr: 0x%08X, size: 0x%08X\n", i, (uint32_t)(mem_table->table[i].addr), (uint32_t)(mem_table->table[i].size));
  }

  // fbr_init_all(vid_table);
  pci_init_all();

  conn_release(tty0);
  
  for (;;);
}
