#ifndef __NOP_IDT_H__
#define __NOP_IDT_H__

typedef struct idt_entry_t idt_entry_t;

struct idt_entry_t {
  uint16_t addr_lo;
  uint16_t sgmt;
  uint8_t zero;
  uint8_t attr;
  uint16_t addr_hi;
} __attribute__((packed));

extern idt_entry_t *idt_table;

#endif