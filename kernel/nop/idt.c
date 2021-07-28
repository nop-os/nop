#include <arch/i586.h>
#include <nop/type.h>
#include <nop/dbg.h>
#include <nop/idt.h>
#include <nop/mem.h>
#include <nop/sys.h>

idt_hand_t *idt_hand = NULL;
size_t idt_count = 0;

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

  // PIC mask
  i586_outb(0x00, 0x0021);
  i586_outb(0x00, 0x00A1);

  // PIT
  i586_outb(0x9C, 0x0040);
  i586_outb(0x2E, 0x0040);

  for (int i = 0; i < 256; i++) {
    idt_table[i].addr_lo = (uint16_t)(idt_list[i] >>  0);
    idt_table[i].addr_hi = (uint16_t)(idt_list[i] >> 16);
  }

  i586_lidt(idt_table);
  i586_sti();
}

size_t idt_add(idt_hand_t hand) {
  for (size_t i = 0; i < idt_count; i++) {
    if (idt_hand[i].id == -1) {
      idt_hand[i] = hand;
      return i;
    }
  }

  idt_hand = mem_realloc(idt_hand, (idt_count + 1) * sizeof(idt_hand_t));
  if (!idt_hand) return 0xFFFFFFFF;

  idt_hand[idt_count] = hand;

  return idt_count++;
}

void idt_remove(size_t index) {
  if (index == idt_count - 1) {
    idt_hand = mem_realloc(idt_hand, (idt_count - 1) * sizeof(idt_hand_t));
    if (!idt_hand) return;

    idt_count--;
  } else {
    idt_hand[index].id = -1;
  }
}

void idt_call(i586_regs_t *regs, int id) {
  for (size_t i = 0; i < idt_count; i++) {
    if (idt_hand[i].id == id) {
      idt_hand[i].func(regs, idt_hand + i);
    }
  }

  if (id < IDT_PIC_BASE) {
    dbg_failf("idt: unhandled interrupt %d\n", id);
    sys_panic();
  }
}