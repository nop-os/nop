#include <arch/i586.h>
#include <nop/real.h>
#include <nop/type.h>
#include <nop/virt.h>
#include <nop/idt.h>
#include <string.h>

int real_read(int drive, uint64_t lba, void *buffer) {
  // TODO: kill myself
  
  real_dap_t *dap = (void *)(0x00007038);
  
  dap->size = 16;
  dap->count = 1;
  dap->addr = 0x00007100;
  dap->lba = lba;
  
  i586_regs_t regs;
  
  regs.eax = 0x00004200;
  regs.esi = 0x00007038;
  regs.esp = 0x00001000;
  regs.edx = drive;
  
  idt_pic_off();
  i586_cli();
  
  int value = i586_int16(0x13, &regs);
  
  virt_load(virt_table);
  i586_lidt(idt_table);
  
  i586_sti();
  idt_pic_on();
  
  if (value) return 0;
  
  memcpy(buffer, (void *)(0x00007100), 512);
  return 1;
}

int real_write(int drive, uint64_t lba, void *buffer) {
  // TODO: kill myself, again
  
  real_dap_t *dap = (void *)(0x00007038);
  memcpy((void *)(0x00007100), buffer, 512);
  
  dap->size = 16;
  dap->count = 1;
  dap->addr = 0x00007100;
  dap->lba = lba;
  
  i586_regs_t regs;
  
  regs.eax = 0x00004300;
  regs.esi = 0x00007038;
  regs.esp = 0x00001000;
  regs.edx = drive;
  
  idt_pic_off();
  i586_cli();
  
  int value = i586_int16(0x13, &regs);
  
  virt_load(virt_table);
  i586_lidt(idt_table);
  
  i586_sti();
  idt_pic_on();
  
  return !value;
}
