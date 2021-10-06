#ifndef __NOP_SEND_H__
#define __NOP_SEND_H__

#include <nop/type.h>

uint32_t nop_type(const char *str);
uint32_t nop_send(int id, const char *type, uint32_t data_1, uint32_t data_2, uint32_t data_3);

int nop_find(const char *prog);
void *nop_phys(int id, uint32_t addr);

#endif
