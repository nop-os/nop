#ifndef __ARCH_I586_H__
#define __ARCH_I586_H__

#include <nop/type.h>

typedef struct i586_regs_t i586_regs_t;

struct i586_regs_t {
  uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi, eip;
} __attribute__((packed));

// String operations

extern void i586_rep_stosb(uint8_t al, uint32_t ecx, void *edi);
extern void i586_rep_stosw(uint16_t ax, uint32_t ecx, void *edi);
extern void i586_rep_stosd(uint32_t eax, uint32_t ecx, void *edi);

extern void i586_rep_movsb(uint32_t ecx, void *edi, const void *esi);
extern void i586_rep_movsw(uint32_t ecx, void *edi, const void *esi);
extern void i586_rep_movsd(uint32_t ecx, void *edi, const void *esi);

extern void i586_rep_insb(uint32_t ecx, uint16_t dx, void *edi);
extern void i586_rep_insw(uint32_t ecx, uint16_t dx, void *edi);
extern void i586_rep_insd(uint32_t ecx, uint16_t dx, void *edi);

extern const void *i586_repe_cmpsb(uint32_t ecx, const void *edi, const void *esi);
extern const void *i586_repe_cmpsw(uint32_t ecx, const void *edi, const void *esi);
extern const void *i586_repe_cmpsd(uint32_t ecx, const void *edi, const void *esi);

extern const void *i586_repne_cmpsb(uint32_t ecx, const void *edi, const void *esi);
extern const void *i586_repne_cmpsw(uint32_t ecx, const void *edi, const void *esi);
extern const void *i586_repne_cmpsd(uint32_t ecx, const void *edi, const void *esi);

extern const void *i586_repe_scasb(uint8_t al, uint32_t ecx, const void *edi);
extern const void *i586_repe_scasw(uint16_t ax, uint32_t ecx, const void *edi);
extern const void *i586_repe_scasd(uint32_t eax, uint32_t ecx, const void *edi);

extern const void *i586_repne_scasb(uint8_t al, uint32_t ecx, const void *edi);
extern const void *i586_repne_scasw(uint16_t ax, uint32_t ecx, const void *edi);
extern const void *i586_repne_scasd(uint32_t eax, uint32_t ecx, const void *edi);

// I/O

extern uint8_t i586_inb(uint16_t dx);
extern uint16_t i586_inw(uint16_t dx);
extern uint32_t i586_ind(uint16_t dx);

extern void i586_outb(uint8_t al, uint16_t dx);
extern void i586_outw(uint16_t ax, uint16_t dx);
extern void i586_outd(uint32_t eax, uint16_t dx);

extern void i586_wait(void);

// Other

extern void i586_set_cr3(const void *edi);
extern void i586_clr_cr3(void);

extern void i586_sti(void);
extern void i586_cli(void);

extern void i586_lidt(const void *edi);

#endif
