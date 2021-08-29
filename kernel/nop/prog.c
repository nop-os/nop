#include <arch/i586.h>
#include <nop/prog.h>
#include <nop/type.h>
#include <nop/virt.h>
#include <nop/idt.h>
#include <nop/mem.h>

prog_t *prog_arr = NULL;
size_t prog_idx = 0;
int prog_id = 0;

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
      
      return i + 1;
    }
  }
  
  return 0;
}

uint32_t prog_call(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  if (!id) {
    return 0;
  }
  
  int (*func)(uint32_t, uint32_t, uint32_t, uint32_t) = (void *)(prog_arr[id - 1].start);
  
  int old_id = prog_id;
  prog_id = id;
  
  virt_load(prog_arr[id - 1].virt_table);
  uint32_t value = func(type, data_1, data_2, data_3);
  
  virt_load(virt_table);
  
  prog_id = old_id;
  return value;
}

void prog_tick(i586_regs_t *regs, idt_hand_t *hand) {
  virt_load(virt_table);
  
  if (!prog_arr) {
    return;
  }
    
  for (int i = 0; i < PROG_MAX; i++) {
    if (!prog_arr[i].free && prog_arr[i].tick) {
      prog_call(0x4B434954, 0, 0, 0, i + 1);
    }
  }
  
  if (prog_id) {
    virt_load(prog_arr[prog_id - 1].virt_table);
  }
}
