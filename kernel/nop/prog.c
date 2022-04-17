#include <exec/aout.h>
#include <nop/alloc.h>
#include <nop/file.h>
#include <nop/page.h>
#include <nop/prog.h>
#include <nop/type.h>
#include <nop/virt.h>
#include <nop/idt.h>

#include <nop/term.h>

prog_t *prog_list = NULL;
int prog_id = 0;

int prog_busy = 0;
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

int prog_load(const char *path, const char **argv, const char **envp) {
  int id = 0;
  
  for (int i = 0; i < PROG_COUNT; i++) {
    if (prog_list[i].free) {
      id = i + 1;
      break;
    }
  }
  
  if (!id) return 0;
  
  int file;
  if (!(file = file_open(path))) return 0;
  
  prog_data_t data;
  
  if (aout_check(file)) {
    data = aout_load(file);
  } else {
    file_close(file, 0);
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
  // regs->esp + 16 -> return address
  
  prog_list[id - 1].regs.esp = (uint32_t)(esp) + 32;
  return id;
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
  if (prog_busy) {
    prog_waiting = 1;
    return;
  }
  
  prog_busy = 1;
  prog_waiting = 0;
  
  if (prog_id && regs) {
    prog_list[prog_id - 1].regs = *regs;
  }
  
  do {
    if (!prog_id) prog_id = PROG_COUNT;
    prog_id = (prog_id % PROG_COUNT) + 1;
  } while (prog_list[prog_id - 1].free || prog_list[prog_id - 1].done);
  
  prog_busy = 0;
  i586_switch(prog_list[prog_id - 1].regs.esp - 32, prog_list[prog_id - 1].table);
}

void prog_return(uint32_t value) {
  prog_list[prog_id - 1].done = 1;
  prog_list[prog_id - 1].value = value;
  
  term_infof("program %d returned %d!\n", prog_id, value);
  prog_switch(NULL);
}
