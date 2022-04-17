#ifndef __NOP_REAL_H__
#define __NOP_REAL_H__

#include <nop/type.h>

// WARNING: the following functions are only for really rare cases where
// ATA PIO is not supported, or where the boot media is not an ATA drive

typedef struct real_dap_t real_dap_t;

struct real_dap_t {
  uint16_t size;
  uint16_t count;
  
  uint32_t addr;
  uint64_t lba;
} __attribute__((packed));

int real_read(int drive, uint64_t lba, void *buffer);
int real_write(int drive, uint64_t lba, void *buffer);

#endif
