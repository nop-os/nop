#include <arch/i586.h>
#include <nop/prog.h>
#include <nop/type.h>
#include <nop/virt.h>
#include <nop/idt.h>
#include <nop/mem.h>

prog_t *prog_arr = NULL;
size_t prog_idx = 0;

void prog_init(void) {
  prog_arr = mem_alloc(PROG_MAX * sizeof(prog_t));
  
  idt_hand_t hand = (idt_hand_t){
    prog_tick,
    NULL,
    IDT_CLK
  };
  
  for (int i = 0; i < PROG_MAX; i++) {
    prog_arr[i].free = 1;
  }
  
  prog_idx = idt_add(hand);
}

int prog_push(prog_t prog) {
  if (!prog_arr) {
    return 0;
  }
  
  for (int i = 0; i < PROG_MAX; i++) {
    if (prog_arr[i].free) {
      prog_arr[i] = prog;
      prog_arr[i].free = 0;
      
      virt_map(prog_arr[i].virt_table, VIRT_NOP_ADDR, VIRT_NOP_ADDR, VIRT_WRITE, VIRT_NOP_SIZE);
      
      return i + 1;
    }
  }
  
  return 0;
}

int prog_call(uint32_t type, void *data, int id) {
  if (!id) {
    return 0;
  }
  
  int (*func)(uint32_t, void *) = (void *)(prog_arr[id - 1].buffer);
  
  virt_load(prog_arr[id - 1].virt_table);
  int value = func(type, data);
  
  virt_load(virt_table);
  return value;
}

void prog_tick(i586_regs_t *regs, idt_hand_t *hand) {
  if (!prog_arr) {
    return;
  }
    
  for (int i = 0; i < PROG_MAX; i++) {
    if (!prog_arr[i].free && prog_arr[i].tick) {
      prog_call(0x4B434954, NULL, i + 1);
    }
  }
}
