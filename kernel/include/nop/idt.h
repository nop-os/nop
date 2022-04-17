#ifndef __NOP_IDT_H__
#define __NOP_IDT_H__

#include <arch/i586.h>
#include <nop/type.h>

#define IDT_ERR_BASE 0x00
#define IDT_PIC_BASE 0x20
#define IDT_NOP_BASE 0x30

#define IDT_CLK   (IDT_PIC_BASE + 0x00)
#define IDT_KEYB  (IDT_PIC_BASE + 0x01)
#define IDT_MOUSE (IDT_PIC_BASE + 0x0C)
#define IDT_RTC   (IDT_PIC_BASE + 0x08)

#define IDT_NOP (IDT_NOP_BASE + 0x00)

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
  void (*func)(i586_regs_t *regs);
  int id;
};

extern idt_entry_t idt_table[];
extern uint32_t idt_list[];

extern idt_hand_t *idt_hand;
extern int idt_count;

extern int idt_level;

void idt_pic_on(void);
void idt_pic_off(void);

void idt_init(void);

int  idt_add(void (*func)(i586_regs_t *regs), int id);
void idt_remove(int index);

void idt_call(i586_regs_t *regs, int id);

#endif
