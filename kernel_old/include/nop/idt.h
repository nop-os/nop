#ifndef __NOP_IDT_H__
#define __NOP_IDT_H__

#include <arch/i586.h>
#include <nop/type.h>

#define IDT_ERR_BASE 0x00
#define IDT_PIC_BASE 0x20
#define IDT_NOP_BASE 0x30

#define IDT_CLK (IDT_PIC_BASE + 0x00)
#define IDT_KBD (IDT_PIC_BASE + 0x01)

typedef struct idt_entry_t idt_entry_t;
typedef struct idt_hand_t idt_hand_t;

struct idt_entry_t {
  uint16_t addr_lo;
  uint16_t sgmt;
  uint8_t zero;
  uint8_t attr;
  uint16_t addr_hi;
} __attribute__((packed));

struct idt_hand_t {
  void (*func)(i586_regs_t *regs, idt_hand_t *hand);
  void *data;
  int id;
};

extern idt_entry_t idt_table[];
extern uint32_t idt_list[];

extern idt_hand_t *idt_hand;
extern size_t idt_count;

void idt_init(void);

size_t idt_add(idt_hand_t hand);
void   idt_remove(size_t index);

void idt_call(i586_regs_t *regs, int id);

#endif