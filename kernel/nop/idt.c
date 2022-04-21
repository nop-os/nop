#include <arch/i586.h>
#include <nop/alloc.h>
#include <nop/term.h>
#include <nop/type.h>
#include <nop/idt.h>

#include <nop/call.h>

idt_hand_t *idt_hand = NULL;
int idt_count = 0;

int idt_level = 0;

void idt_pic_on(void) {
  i586_outb(0x00, 0x0021);
  i586_outb(0x00, 0x00A1);
  
  i586_outb(0x35, 0x0040);
  i586_outb(0x12, 0x0040);
}

void idt_pic_off(void) {
  i586_outb(0xFF, 0x0021);
  i586_outb(0xFF, 0x00A1);
}

void idt_init(void) {
  // PIC relocation
  i586_outb(0x11, 0x0020);
  i586_wait();
  i586_outb(0x11, 0x00A0);
  i586_wait();
  i586_outb(0x20, 0x0021);
  i586_wait();
  i586_outb(0x28, 0x00A1);
  i586_wait();
  i586_outb(0x04, 0x0021);
  i586_wait();
  i586_outb(0x02, 0x00A1);
  i586_wait();
  i586_outb(0x01, 0x0021);
  i586_wait();
  i586_outb(0x01, 0x00A1);
  i586_wait();
  
  // PIC(and PIT)
  idt_pic_on();
  
  for (int i = 0; i < 256; i++) {
    idt_table[i].addr_lo = (uint16_t)(idt_list[i] >>  0);
    idt_table[i].addr_hi = (uint16_t)(idt_list[i] >> 16);
  }

  i586_lidt(idt_table);
  i586_sti();
  
  term_infof("initialized interrupts\n");
}

int idt_add(void (*func)(i586_regs_t *regs), int id) {
  for (int i = 0; i < idt_count; i++) {
    if (idt_hand[i].id == -1) {
      idt_hand[i].func = func;
      idt_hand[i].id = id;
      
      return i;
    }
  }

  idt_hand = realloc(idt_hand, (idt_count + 1) * sizeof(idt_hand_t));
  if (!idt_hand) return 0xFFFFFFFF;

  idt_hand[idt_count].func = func;
  idt_hand[idt_count].id = id;

  return idt_count++;
}

void idt_remove(int index) {
  if (index == idt_count - 1) {
    idt_hand = realloc(idt_hand, (idt_count - 1) * sizeof(idt_hand_t));
    if (!idt_hand) return;

    idt_count--;
  } else {
    idt_hand[index].id = -1;
  }
}

void idt_call(i586_regs_t *regs, int id) {
  idt_level++;
  
  if (id < 32) {
    term_failf("(%d) exception %d!\n", call_flag, id);
    
    i586_cli();
    for (;;);
  }
  
  for (int i = 0; i < idt_count; i++) {
    if (idt_hand[i].id == id) {
      idt_hand[i].func(regs);
    }
  }
  
  idt_level--;
}
