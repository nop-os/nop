#ifndef __DEVS_ATA_H__
#define __DEVS_ATA_H__

#include <nop/type.h>
#include <nop/conn.h>

#define ATA_PRIMARY   0x01F0
#define ATA_SECONDARY 0x0170

typedef struct ata_t ata_t;

typedef struct ata_part_t ata_part_t;
typedef struct ata_mbr_t ata_mbr_t;

struct ata_t {
  uint8_t drive;
  uint64_t size;

  char serial[21];
};

struct ata_part_t {
  uint8_t attr;
  uint32_t chs_addr: 24;

  uint8_t type;
  uint32_t chs_size: 24;

  uint32_t lba_addr;
  uint32_t lba_size;
} __attribute__((packed));

struct ata_mbr_t {
  ata_part_t part[4];
  uint16_t sign;
} __attribute__((packed));

extern conn_hand_t ata_hand;

ssize_t ata_init(conn_hand_t *hand, void *data);
void    ata_free(conn_hand_t *hand);

void    ata_connect(conn_t *conn, const char *path);

ssize_t ata_write(conn_t *conn, void *buffer, size_t count);
ssize_t ata_read(conn_t *conn, void *buffer, size_t count);

void ata_init_all(void);

#endif