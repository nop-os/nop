#include <arch/i586.h>
#include <nop/alloc.h>
#include <nop/call.h>
#include <nop/page.h>
#include <nop/prog.h>
#include <nop/term.h>
#include <nop/virt.h>
#include <nop/idt.h>
#include <string.h>

const call_t call_array[] = {
  (call_t){"term_write", 0, (void *)($term_write)}
};

const int call_count = sizeof(call_array) / sizeof(call_t);

static char call_name[32];

void call_init(void) {
  idt_add(call_handle, IDT_NOP);
}

int call_find(const char *name, int prog, call_t *call) {
  if (prog) {
    for (int i = 0; i < prog_list[prog - 1].call_count; i++) {
      if (!strcmp(prog_list[prog - 1].call_array[i].name, name)) {
        *call = prog_list[prog - 1].call_array[i];
        return 1;
      }
    }
    
    return call_find(name, prog_list[prog - 1].parent, call);
  } else {
    for (int i = 0; i < call_count; i++) {
      if (!strcmp(call_array[i].name, name)) {
        *call = call_array[i];
        return 1;
      }
    }
    
    return 0;
  }
}

void call_handle(i586_regs_t *regs) {
  // TODO: make faster, find a way to avoid needing that prog_waiting flag, etc.
  
  strncpy(call_name, (void *)(regs->esi), 32);
  call_name[31] = '\0';
  
  uint32_t *esp = CALL_STACK_ADDR;
  uint32_t *table = virt_table;
  
  call_t call;
  
  if (call_find(call_name, prog_id, &call)) {
    uint32_t eip = (uint32_t)(call.func);
    int old_prog = prog_id;
    
    if (call.prog) {
      esp = malloc(CALL_STACK_SIZE) + CALL_STACK_SIZE;
      table = prog_list[call.prog - 1].table;
      
      prog_id = call.prog;
    }
    
    // all the 0 ones will be filled in by call_switch
    
    *(--esp) = 0x00000000; // esp
    *(--esp) = 0x00000000; // table
    
    *(--esp) = regs->edi;
    *(--esp) = regs->edx;
    *(--esp) = regs->ecx;
    *(--esp) = regs->ebx;
    *(--esp) = regs->eax;
    
    *(--esp) = 0x00000000; // return address
    *(--esp) = eip;        // eip
    
    regs->eax = call_switch((uint32_t)(esp), table);
    prog_id = old_prog;
  }
  
  if (prog_waiting) {
    prog_switch(regs);
  }
}
