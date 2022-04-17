#ifndef __NOP_CALL_H__
#define __NOP_CALL_H__

#include <arch/i586.h>

void call_init(void);
void call_handle(i586_regs_t *regs);

extern uint32_t call_switch(uint32_t eax, const void *edi);

#endif
