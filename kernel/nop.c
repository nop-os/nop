#include <boot/tinyboot.h>
#include <nop/page.h>
#include <nop/prog.h>
#include <nop/syst.h>
#include <nop/virt.h>
#include <nop/ata.h>
#include <nop/dbg.h>
#include <nop/fat.h>
#include <nop/idt.h>
#include <nop/mem.h>

__attribute__((__section__(".entry"), __used__))
void nop(tb_mem_t *mem_table, tb_vid_t *vid_table) {
  dbg_init(0x03F8);
  
  page_init(mem_table);
  virt_init();
  mem_init(page_size / 512);
  idt_init();
  ata_init();
  fat_init();
  prog_init();
  syst_init();
  
  dbg_infof("hello, world!\n");
  dbg_infof("config.txt cluster: 0x%08X\n", fat_find(0, 0, NULL, "config.txt"));
  
  int init_id = syst_open("0:/syst/init.txt");
  
  if (init_id <= 0 || init_id > SYST_OPEN_MAX) {
    dbg_failf("could not open 0:/syst/init.txt\n");
    dbg_panic();
  }
  
  char prog_buf[FAT_PATH_MAX];
  int prog_len = 0;
  
  char c;
  
  dbg_infof("config buffer: 0x%08X\n", syst_files[init_id - 1].buffer);
  
  while (syst_read(init_id, &c, 1)) {
    prog_buf[prog_len] = '\0';
    
    if (c == '\n') {
      if (prog_len) {
        int new_id = syst_load(prog_buf);
      
        if (new_id <= 0 || new_id > SYST_OPEN_MAX) {
          dbg_failf("could not open %s\n", prog_buf);
          dbg_panic();
        }
        
        prog_call(new_id, PROG_INIT, (uint32_t)(vid_table), 0, 0);
      }
      
      prog_len = 0;
    } else if (prog_len < FAT_PATH_MAX - 1) {
      prog_buf[prog_len++] = c;
    }
  }
  
  dbg_infof("done reading 0:/syst/init.txt!\n");
  
  syst_clos(init_id);
  for (;;);
}
