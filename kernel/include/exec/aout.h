#ifndef __EXEC_AOUT_H__
#define __EXEC_AOUT_H__

#include <nop/prog.h>
#include <nop/type.h>

typedef struct aout_t aout_t;

struct aout_t {
  uint8_t info[4];
  uint32_t text;
  uint32_t data;
  uint32_t bss;
  uint32_t syms;
  uint32_t entry;
  uint32_t trsize;
  uint32_t drsize;
};

#define AOUT_MAGIC(aout) (((uint16_t)((aout).info[3]) << 0) | ((uint16_t)((aout).info[2]) << 8))
#define AOUT_TARGET(aout) ((((uint16_t)((aout).info[1]) << 0) | ((uint16_t)((aout).info[0]) << 8)) & 0x03FF)

int         aout_check(int file);
prog_data_t aout_load(int file);

#endif
