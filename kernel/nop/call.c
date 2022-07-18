#include <arch/i586.h>
#include <nop/call.h>
#include <nop/page.h>
#include <nop/prog.h>
#include <nop/virt.h>
#include <nop/idt.h>
#include <string.h>

const call_t call_array[] = {
  (call_t){"file_open", 0, (void *)($file_open)},
  (call_t){"file_create", 0, (void *)($file_create)},
  (call_t){"file_delete", 0, (void *)($file_delete)},
  (call_t){"file_close", 0, (void *)($file_close)},
  (call_t){"file_write", 0, (void *)($file_write)},
  (call_t){"file_read", 0, (void *)($file_read)},
  (call_t){"file_entry", 0, (void *)($file_entry)},
  (call_t){"file_seek", 0, (void *)($file_seek)},
  (call_t){"file_resize", 0, (void *)($file_resize)},
  (call_t){"file_getmode", 0, (void *)($file_getmode)},
  (call_t){"file_setmode", 0, (void *)($file_setmode)},
  
  (call_t){"term_write", 0, (void *)($term_write)},
  (call_t){"term_read", 0, (void *)($term_read)},
  (call_t){"term_getmode", 0, (void *)($term_getmode)},
  (call_t){"term_setmode", 0, (void *)($term_setmode)},
  (call_t){"term_getsize", 0, (void *)($term_getsize)},
  
  (call_t){"prog_load", 0, (void *)($prog_load)},
  (call_t){"prog_kill", 0, (void *)($prog_kill)},
  (call_t){"prog_wait", 0, (void *)($prog_wait)},
  (call_t){"prog_skip", 0, (void *)($prog_skip)},
  (call_t){"prog_alloc", 0, (void *)($prog_alloc)},
  (call_t){"prog_check", 0, (void *)($prog_check)},
  
  (call_t){"call_read", 0, (void *)($call_read)},
  (call_t){"call_write", 0, (void *)($call_write)},
  (call_t){"call_getid", 0, (void *)($call_getid)},
};

const int call_count = sizeof(call_array) / sizeof(call_t);

int call_stack[64];
int call_offset = 0;

int call_flag = 0;

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

void call_kernel(void *func, uint32_t value) {
  if (call_flag) {
    ((void (*)(uint32_t))(func))(value);
    return;
  }
  
  uint32_t *esp = CALL_STACK_TERM;
  uint32_t eip = (uint32_t)(func);
  
  // all the 0 ones will be filled in by call_switch
  
  *(--esp) = 0x00000000; // esp
  *(--esp) = 0x00000000; // table
  
  *(--esp) = 0x00000000;
  *(--esp) = 0x00000000;
  *(--esp) = 0x00000000;
  *(--esp) = 0x00000000;
  *(--esp) = value;
  
  *(--esp) = 0x00000000; // return address
  *(--esp) = eip;        // eip
  
  call_flag = 1;
  call_switch((uint32_t)(esp), virt_table);
  call_flag = 0;
}

void call_handle(i586_regs_t *regs) {
  // TODO: make FASTER
  
  strncpy(call_name, (void *)(regs->esi), 32);
  call_name[31] = '\0';
  
  uint32_t *old_esp = CALL_STACK_ADDR;
  uint32_t *esp = old_esp;
  
  uint32_t *table = virt_table;
  
  call_t call;
  
  if (call_find(call_name, prog_id, &call)) {
    uint32_t eip = (uint32_t)(call.func);
    int old_prog = prog_id;
    
    call_stack[call_offset++] = old_prog;
    
    if (call.prog) {
      old_esp = esp = (void *)(page_alloc(CALL_STACK_SIZE >> 12, 1) + CALL_STACK_SIZE);
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
    
    if (!call.prog) call_flag = 1;
    regs->eax = call_switch((uint32_t)(esp), table);
    if (!call.prog) call_flag = 0;
    
    prog_id = old_prog;
    call_offset--;
    
    if (call.prog) {
      page_free((void *)(old_esp) - CALL_STACK_SIZE, CALL_STACK_SIZE >> 12);
    }
  }
  
  if (prog_waiting) {
    prog_switch(regs);
  }
}
