#include <arch/i586.h>
#include <devs/ata.h>
#include <nop/type.h>
#include <nop/conn.h>
#include <nop/dbg.h>
#include <nop/mem.h>

conn_hand_t ata_hand = (conn_hand_t){
  0, "tty", NULL,
  ata_init, NULL,
  ata_connect, NULL,
  ata_write, ata_read, NULL
};

ssize_t ata_init(conn_hand_t *hand, void *data) {
  uint8_t drive = (uint8_t)((size_t)(data));

  uint16_t port = (drive & 0x02) ? ATA_SECONDARY : ATA_PRIMARY;
  uint8_t slot = (drive & 0x01) ? 0xB0 : 0xA0;

  uint8_t status = i586_inb(port + 0x07);

  if (status == 0xFF) {
    dbg_failf("ata: drive %d is floating\n", drive);
    return 0;
  }

  i586_outb(slot, port + 0x06);
  i586_outw(0x0000, port + 0x02);
  i586_outw(0x0000, port + 0x03);
  i586_outw(0x0000, port + 0x04);
  i586_outw(0x0000, port + 0x05);
  i586_outb(0xEC, port + 0x07);

  for (;;) {
    status = i586_inb(port + 0x07);

    if (!status) {
      dbg_failf("ata: drive %d status is 0x00\n", drive);
      return 0;
    }

    if (!(status & 0x80)) break;
  }

  for (;;) {
    status = i586_inb(port + 0x07);

    if (status & 0x01) {
      dbg_failf("ata: drive %d failed\n", drive);
      return 0;
    }

    if (status & 0x08) break;
  }

  uint16_t buffer[256];

  for (int i = 0; i < 256; i++) {
    buffer[i] = i586_inw(port + 0x00);
  }

  ata_t *ata = mem_alloc(sizeof(ata_t));
  ata->drive = drive;
  ata->size = ((uint64_t)(buffer[0x66]) << 32) | ((uint64_t)(buffer[0x65]) << 16) | buffer[0x64];

  hand->data = ata;

  dbg_donef("ata: drive %d initialized\n", drive);
  dbg_infof("- size: %d MiB\n", (uint32_t)(ata->size >> 11));

  return 1;
}

void ata_connect(conn_t *conn, const char *path) {
  ata_t *ata = conn->handler->data;

  conn->node.name[0] = '\0';
  conn->node.attr = conn_attr_write | conn_attr_read | conn_attr_seek;
  conn->node.size = ata->size << 9;

  conn->offset = 0;
  conn->data = conn->handler->data;
}

int ata_write_sector(conn_t *conn, uint64_t lba, void *buffer) {

}

int ata_read_sector(conn_t *conn, uint64_t lba, void *buffer) {
  ata_t *ata = conn->handler->data;

  uint16_t port = (ata->drive & 0x02) ? ATA_SECONDARY : ATA_PRIMARY;
  uint8_t slot = (ata->drive & 0x01) ? 0x50 : 0x40;

  i586_outb(slot, port + 0x06);
  i586_outb(0x00, port + 0x02);
  i586_outb((uint8_t)(lba >> 24), port + 0x03);
  i586_outb((uint8_t)(lba >> 32), port + 0x04);
  i586_outb((uint8_t)(lba >> 40), port + 0x05);
  i586_outb(0x01, port + 0x02);
  i586_outb((uint8_t)(lba >>  0), port + 0x03);
  i586_outb((uint8_t)(lba >>  8), port + 0x04);
  i586_outb((uint8_t)(lba >> 16), port + 0x05);

  uint8_t status;

  for (;;) {
    status = i586_inb(port + 0x07);

    if (!status) return 0;
    if (!(status & 0x80)) break;
  }

  for (;;) {
    status = i586_inb(port + 0x07);

    if (status & 0x01) return 0;
    if (status & 0x08) break;
  }

  uint16_t *buffer_16 = buffer;

  for (int i = 0; i < 256; i++) {
    buffer_16[i] = i586_inw(port + 0x00);
  }

  for (int i = 0; i < 14; i++) {
    i586_inb(port + 0x07);
  }

  return 1;
}

ssize_t ata_write(conn_t *conn, void *buffer, size_t count) {

}

ssize_t ata_read(conn_t *conn, void *buffer, size_t count) {

}

void ata_init_all(void) {
  ssize_t count = 0;

  for (uint16_t i = 0; i < 4; i++) {
    void *data = (void *)((size_t)(i));
    count += conn_init(ata_hand, data);
  }

  dbg_infof("ata: %d drives initialized\n", count);
}