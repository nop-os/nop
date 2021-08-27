#include <boot/tinyboot.h>
#include <nop/page.h>
#include <nop/prog.h>
// #include <nop/syst.h>
#include <nop/virt.h>
#include <nop/ata.h>
#include <nop/dbg.h>
#include <nop/fat.h>
#include <nop/idt.h>
#include <nop/mem.h>

int test_1(uint32_t type, void *data) {
  if (type == 0x4B434954) {
    dbg_infof("hey, i'm program number 1!\n");
  }
  
  return 0;
}

int test_2(uint32_t type, void *data) {
  if (type == 0x4B434954) {
    dbg_infof("hey, i'm program number 2!\n");
  }
  
  return 0;
}

int test_3(uint32_t type, void *data) {
  if (type == 0x4B434954) {
    dbg_infof("hey, i'm program number 3!\n");
  }
  
  return 0;
}

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
  
  dbg_infof("hello, world!\n");
  dbg_infof("config.txt cluster: 0x%08X\n", fat_find(0, 0, NULL, "config.txt"));
  
  // int init_id = syst_open("0:/syst/init.txt");
  
  // if (!init_id) {
  //   dbg_failf("could not open 0:/syst/init.txt\n");
  //   dbg_panic();
  // }
  
  char prog_buf[PATH_MAX];
  int prog_len = 0;
  
  char c;
  
  /*
  
  while (syst_read(init_id, &c, 1)) {
    prog_buf[prog_len] = '\0';
    
    if (c == '\n') {
      if (prog_len) {
        int prog_id = syst_load(prog_buf);
      
        if (!prog_id) {
          dbg_failf("could not open %s\n", prog_buf);
          dbg_panic();
        }
        
        syst_send(prog_id, "INIT", vid_table);
      }
      
      prog_len = 0;
    } else if (prog_len < PATH_MAX - 1) {
      prog_buf[prog_len++] = c;
    }
  }
  
  syst_clos(init_id);
  */
  
  for (;;);
  
  prog_t prog = (prog_t){
    virt_table,
    test_1,
    0, 0, 1
  };
  
  int id_1 = prog_push(prog);
  
  prog.buffer = test_2;
  
  int id_2 = prog_push(prog);
  
  prog.buffer = test_3;
  
  int id_3 = prog_push(prog);
  
  for (;;);
}
