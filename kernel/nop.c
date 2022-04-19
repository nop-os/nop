#include <boot/tinyboot.h>
#include <nop/call.h>
#include <nop/file.h>
#include <nop/page.h>
#include <nop/prog.h>
#include <nop/term.h>
#include <nop/time.h>
#include <nop/virt.h>
#include <nop/ata.h>
#include <nop/fat.h>
#include <nop/idt.h>
#include <nop/ps2.h>
#include <config.h>
#include <string.h>

__attribute__((__section__(".entry"), __used__))
void nop(tb_mem_t *mem_table, tb_vid_t *vid_table) {
  term_init(vid_table);
  
  page_init(mem_table);
  virt_init();
  idt_init();
  time_init();
  ata_init();
  fat_init();
  prog_init();
  call_init();
  
  int config = file_open("0:/syst/npconfig.txt");
  
  if (!config) {
    term_failf("cannot open '0:/syst/npconfig.txt'\n");
    term_panic();
  }
  
  char buffer[FILE_PATH_MAX];
  
  if (cfg_read_str(config, "layout", buffer)) {
    ps2_init(buffer);
  }
  
  for (int i = 0; i < cfg_read_arr_len(config, "init"); i++) {
    const char *argv[1] = {NULL};
    const char *envp[1] = {NULL};
    
    // TODO: buffer will contain the entire command, we should separate the args!
    
    cfg_read_arr_str(config, "init", i, buffer);
    prog_load(buffer, argv, envp, NULL, 0);
  }
  
  // don't forget to close the file, as the user may want to open it too!
  file_close(config, 0);
  
  prog_start(); // start scheduler
  for (;;);
}
