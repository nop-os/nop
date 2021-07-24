#ifndef __DEVS_ATA_H__
#define __DEVS_ATA_H__

#include <nop/type.h>
#include <nop/conn.h>

#define ATA_PRIMARY   0x01F0
#define ATA_SECONDARY 0x0170

typedef struct ata_t ata_t;

struct ata_t {
  uint8_t drive;
  uint64_t size;
};

extern conn_hand_t ata_hand;

ssize_t ata_init(conn_hand_t *hand, void *data);
void    ata_connect(conn_t *conn, const char *path);

ssize_t ata_write(conn_t *conn, void *buffer, size_t count);
ssize_t ata_read(conn_t *conn, void *buffer, size_t count);

#endif