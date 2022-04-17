#include <arch/i586.h>
#include <nop/call.h>
#include <nop/page.h>
#include <nop/prog.h>
#include <nop/term.h>
#include <nop/virt.h>
#include <nop/idt.h>
#include <string.h>

static char call_name[32];

void call_init(void) {
  idt_add(call_handle, IDT_NOP);
}

uint32_t call_test(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx) {
  term_infof("%s\n", call_name);
  return 1;
}

void call_handle(i586_regs_t *regs) {
  prog_busy = 1;
  uint32_t *esp = (uint32_t *)(0x00007000);
  
  strncpy(call_name, (void *)(regs->esi), 32);
  call_name[31] = '\0';
  
  // all the 0 ones will be filled in by call_switch
  
  *(--esp) = 0x00000000; // esp
  *(--esp) = 0x00000000; // table
  
  *(--esp) = regs->edx;
  *(--esp) = regs->ecx;
  *(--esp) = regs->ebx;
  *(--esp) = regs->eax;
  
  *(--esp) = 0x00000000; // return address
  *(--esp) = (uint32_t)(call_test); // eip
  
  regs->eax = call_switch((uint32_t)(esp), virt_table);
  prog_busy = 0;
  
  if (prog_waiting) {
    prog_switch(regs);
  }
}
