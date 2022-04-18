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
  
  // ps2_init("0:/data/keyboard.bin");
  
  const char *argv[1] = {NULL};
  const char *envp[1] = {NULL};
  
  prog_load("0:/prog/test.out", argv, envp, NULL, 0);
  
  prog_start(); // start scheduler
  for (;;);
}
