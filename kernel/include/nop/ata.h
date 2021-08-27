#ifndef __NOP_ATA_H__
#define __NOP_ATA_H__

#include <nop/type.h>

#define ATA_PRIMARY   0x01F0
#define ATA_SECONDARY 0x0170

typedef struct ata_t ata_t;

typedef struct ata_part_t ata_part_t;
typedef struct ata_mbr_t ata_mbr_t;

struct ata_t {
  int present;
  uint64_t size;

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

extern ata_t ata_drives[];

void ata_init(void);
int  ata_read(int drive, uint64_t lba, void *buffer, size_t count);
int  ata_write(int drive, uint64_t lba, void *buffer, size_t count);

#endif
