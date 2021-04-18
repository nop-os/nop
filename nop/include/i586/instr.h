#ifndef __I586_INSTR_H__
#define __I586_INSTR_H__

#include <stdint.h>

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

extern const void *i586_repe_cmpsb(const void *edi, const void *esi);
extern const void *i586_repe_cmpsw(const void *edi, const void *esi);
extern const void *i586_repe_cmpsd(const void *edi, const void *esi);

extern const void *i586_repne_cmpsb(const void *edi, const void *esi);
extern const void *i586_repne_cmpsw(const void *edi, const void *esi);
extern const void *i586_repne_cmpsd(const void *edi, const void *esi);

extern const void *i586_repe_scasb(uint8_t al, const void *edi);
extern const void *i586_repe_scasw(uint16_t ax, const void *edi);
extern const void *i586_repe_scasd(uint32_t eax, const void *edi);

extern const void *i586_repne_scasb(uint8_t al, const void *edi);
extern const void *i586_repne_scasw(uint16_t ax, const void *edi);
extern const void *i586_repne_scasd(uint32_t eax, const void *edi);

// I/O

extern uint8_t i586_inb(uint16_t dx);
extern uint16_t i586_inw(uint16_t dx);
extern uint32_t i586_ind(uint16_t dx);

extern void i586_outb(uint8_t al, uint16_t dx);
extern void i586_outw(uint16_t ax, uint16_t dx);
extern void i586_outd(uint32_t eax, uint16_t dx);

// Other

// TODO: Add things like LIDT, CLI, INT, etc. here?

#endif
