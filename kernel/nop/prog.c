#include <arch/i586.h>
#include <nop/prog.h>
#include <nop/type.h>
#include <nop/virt.h>
#include <nop/dbg.h>
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
  if (id <= 0 || id > PROG_MAX) {
    return 0;
  }
  
  if (prog_arr[id - 1].free) {
    return 0;
  }
  
  int (*func)(int, uint32_t, uint32_t, uint32_t, uint32_t) = prog_arr[id - 1].start;
  
  int old_id = prog_id;
  prog_id = id;
  
  // TODO: make faster by allocating virtual memory maps!
  
  virt_map(virt_table, prog_arr[id - 1].buffer, (void *)(VIRT_NOP_PROG), VIRT_WRITE, (prog_arr[id - 1].size + 0x0FFF) >> 12);
  virt_load(virt_table);
  
  uint32_t value = func(old_id, type, data_1, data_2, data_3);
  
  if (old_id) {
    virt_map(virt_table, prog_arr[old_id - 1].buffer, (void *)(VIRT_NOP_PROG), VIRT_WRITE, (prog_arr[old_id - 1].size + 0x0FFF) >> 12);
    virt_load(virt_table);
  }
  
  prog_id = old_id;
  return value;
}

void prog_tick(i586_regs_t *regs, idt_hand_t *hand) {
  if (!prog_arr) {
    return;
  }
  
  for (int i = 0; i < PROG_MAX; i++) {
    if (!prog_arr[i].free && prog_arr[i].tick) {
      prog_call(i + 1, PROG_TICK, 0, 0, 0);
    }
  }
}
