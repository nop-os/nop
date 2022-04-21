#include <exec/aout.h>
#include <nop/alloc.h>
#include <nop/call.h>
#include <nop/file.h>
#include <nop/page.h>
#include <nop/prog.h>
#include <nop/type.h>
#include <nop/virt.h>
#include <nop/idt.h>

prog_t *prog_list = NULL;
int prog_id = 0;

int prog_waiting = 0;

void prog_init(void) {
  prog_list = malloc(PROG_COUNT * sizeof(prog_t));
  
  for (int i = 0; i < PROG_COUNT; i++) {
    prog_list[i].free = 1;
  }
}

void prog_start(void) {
  idt_add(prog_switch, IDT_CLK);
}

static int prog_get_free(void) {
  for (int i = 0; i < PROG_COUNT; i++) {
    if (prog_list[i].free) return i + 1;
  }
  
  return 0;
}

static void prog_unload(const char **argv, const char **envp, call_t *call_array, int call_count) {
  if (call_array && call_count) {
    free(call_array);
  }
  
  if (argv) {
    int argc = 0;
    
    while (argv[argc]) {
      free((void *)(argv[argc++]));
    }
    
    free(argv);
  }
  
  if (envp) {
    int envc = 0;
    
    while (envp[envc]) {
      free((void *)(envp[envc++]));
    }
    
    free(envp);
  }
}

int prog_load(const char *path, const char **argv, const char **envp, call_t *call_array, int call_count) {
  int id = 0;
  
  for (int i = 0; i < PROG_COUNT; i++) {
    if (prog_list[i].free) {
      id = i + 1;
      break;
    }
  }
  
  if (!id) {
    prog_unload(argv, envp, call_array, call_count);
    return 0;
  }
  
  int file;
  
  if (!(file = file_open(path))) {
    prog_unload(argv, envp, call_array, call_count);
    return 0;
  }
  
  prog_data_t data;
  
  if (aout_check(file)) {
    data = aout_load(file);
  } else {
    file_close(file, 0);
    
    prog_unload(argv, envp, call_array, call_count);
    return 0;
  }
  
  file_close(file, 0);
  
  prog_list[id - 1].data = data;
  
  // TODO: make table smaller by leaving parts empty
  prog_list[id - 1].table = virt_alloc();
  
  virt_map(prog_list[id - 1].table, VIRT_NOP_KERN, VIRT_NOP_KERN, VIRT_WRITE, ((uint32_t)(VIRT_NOP_DATA) - (uint32_t)(VIRT_NOP_KERN)) >> 12);
  virt_map(prog_list[id - 1].table, data.data, VIRT_NOP_CODE, VIRT_WRITE, (data.size + 0x0FFF) >> 12);
  
  prog_list[id - 1].page_count = 0;
  
  prog_list[id - 1].free = 0;
  prog_list[id - 1].done = 0;
  prog_list[id - 1].wait = 0;
  
  prog_list[id - 1].parent = prog_id;
  
  if (!call_array) call_count = 0;
  if (!call_count) call_array = NULL;
  
  prog_list[id - 1].call_array = call_array;
  prog_list[id - 1].call_count = call_count;
  
  const size_t stack_size = 65536;
  
  prog_alloc(id, stack_size >> 12);
  uint32_t *esp = VIRT_NOP_DATA + stack_size;
  
  *((uint32_t *)(virt_phys(prog_list[id - 1].table, --esp))) = (uint32_t)(envp);        // envp
  *((uint32_t *)(virt_phys(prog_list[id - 1].table, --esp))) = (uint32_t)(argv);        // argv
  *((uint32_t *)(virt_phys(prog_list[id - 1].table, --esp))) = (uint32_t)(prog_handle); // return handler
  
  *((uint32_t *)(virt_phys(prog_list[id - 1].table, --esp))) = 0x00000202;              // eflags
  *((uint32_t *)(virt_phys(prog_list[id - 1].table, --esp))) = 0x0008;                  // cs
  *((uint32_t *)(virt_phys(prog_list[id - 1].table, --esp))) = (uint32_t)(data.entry);  // eip
  
  for (int i = 0; i < 8; i++) {
    *((uint32_t *)(virt_phys(prog_list[id - 1].table, --esp))) = 0x00000000; // all the other registers
  }
  
  // regs struct:
  // regs->esp - 32 -> edi
  // regs->esp - 28 -> esi
  // regs->esp - 24 -> ebp
  // regs->esp - 20 -> esp(regs->esp)
  // regs->esp - 16 -> ebx
  // regs->esp - 12 -> edx
  // regs->esp -  8 -> ecx
  // regs->esp -  4 -> eax
  // regs->esp +  0 -> eip
  // regs->esp +  4 -> cs
  // regs->esp +  8 -> eflags
  // regs->esp + 12 -> return address
  
  prog_list[id - 1].regs.esp = (uint32_t)(esp) + 32;
  return id;
}

int prog_kill(int id) {
  if (!id) return 0;
  if (prog_list[id - 1].free) return 0;
  
  for (int i = 0; i < PROG_COUNT; i++) {
    if (prog_list[i].parent == id) {
      prog_kill(i + 1);
    }
  }
  
  prog_list[id - 1].free = 1;
  
  prog_unload(prog_list[id - 1].argv, prog_list[id - 1].envp, prog_list[id - 1].call_array, prog_list[id - 1].call_count);
  page_free(prog_list[id - 1].data.data, (prog_list[id - 1].data.size + 0));
  
  for (int i = 0; i < prog_list[id - 1].page_count; i++) {
    // TODO: find memory space pages to free in table
  }
  
  virt_free(prog_list[id - 1].table);
  return 1;
}

int prog_alloc(int id, size_t count) {
  if (!id) return 0;
  if (prog_list[id - 1].free) return 0;
  
  while (count--) {
    void *page = page_alloc(1, 0);
    
    virt_map(prog_list[id - 1].table, page, VIRT_NOP_DATA + (prog_list[id - 1].page_count << 12), VIRT_WRITE, 1);
    prog_list[id - 1].page_count++;
  }
  
  return 1;
}

void prog_switch(i586_regs_t *regs) {
  if (call_flag || idt_level > 1) {
    prog_waiting = 1;
    return;
  }
  
  prog_waiting = 0;
  
  if (prog_id && regs) {
    prog_list[prog_id - 1].regs = *regs;
  }
  
  int old_id = prog_id;
  
  do {
    if (!prog_id) prog_id = PROG_COUNT;
    prog_id = (prog_id % PROG_COUNT) + 1;
  } while (prog_list[prog_id - 1].free || prog_list[prog_id - 1].done || prog_list[prog_id - 1].wait);
  
  idt_level--;
  i586_switch(prog_list[prog_id - 1].regs.esp - 32, prog_list[prog_id - 1].table);
}

static void __prog_return(uint32_t value) {
  for (int i = 0; i < PROG_COUNT; i++) {
    if (prog_list[i].wait == prog_id) {
      *((uint32_t *)(virt_phys(prog_list[i].table, (void *)(prog_list[i].regs.esp - 4)))) = value;
      prog_list[i].wait = 0;
    }
  }
  
  prog_list[prog_id - 1].done = 1;
  prog_list[prog_id - 1].value = value;
}

void prog_return(uint32_t value) {
  call_kernel(__prog_return, value);
  prog_switch(NULL);
}
