#ifndef __NOP_ATA_H__
#define __NOP_ATA_H__

#include <nop/type.h>

#define ATA_PRIMARY    0x01F0
#define ATA_SECONDARY  0x0170
#define ATA_TERTIARY   0x01E8
#define ATA_QUATERNARY 0x0168

#define ATA_CTRL_PRIMARY    0x03F6
#define ATA_CTRL_SECONDARY  0x0376
#define ATA_CTRL_TERTIARY   0x03E6
#define ATA_CTRL_QUATERNARY 0x0366

#define ATA_INVALID 0xFFFFFFFFFFFFFFFF
#define ATA_CACHE   64

typedef struct ata_t ata_t;

typedef struct ata_part_t ata_part_t;
typedef struct ata_mbr_t ata_mbr_t;

typedef struct ata_cache_t ata_cache_t;

struct ata_t {
  int present;
  uint64_t size;
  
  ata_cache_t *cache;
  char serial[21];
};

struct ata_part_t {
  uint32_t attr    :  8;
  uint32_t chs_addr: 24;

  uint32_t type    :  8;
  uint32_t chs_size: 24;

  uint32_t lba_addr;
  uint32_t lba_size;
} __attribute__((packed));

struct ata_mbr_t {
  uint8_t data[494];
  ata_part_t part[4];
  uint16_t sign;
} __attribute__((packed));

struct ata_cache_t {
  uint64_t addr;
  uint8_t data[512];
} __attribute__((packed));

extern ata_t ata_drives[];

void ata_init(void);
void ata_reset(int drive);

int ata_read(int drive, uint64_t lba, void *buffer, size_t count);
int ata_write(int drive, uint64_t lba, void *buffer, size_t count);

#endif
