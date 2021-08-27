#include <arch/i586.h>
#include <nop/type.h>
#include <nop/ata.h>
#include <nop/dbg.h>
#include <string.h>

ata_t ata_drives[4];
  
static void ata_init_drive(uint8_t drive) {
  uint16_t port = (drive & 0x02) ? ATA_SECONDARY : ATA_PRIMARY;
  uint8_t slot = (drive & 0x01) ? 0xB0 : 0xA0;

  uint8_t status = i586_inb(port + 0x07);

  if (status == 0xFF) {
    dbg_failf("ata: drive %d is floating\n", drive);
    
    ata_drives[drive].present = 0;
    return;
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
      dbg_failf("ata: drive %d has invalid status\n", drive);
      
      ata_drives[drive].present = 0;
      return;
    }

    if (!(status & 0x80)) break;
  }

  for (;;) {
    status = i586_inb(port + 0x07);

    if (status & 0x01) {
      dbg_failf("ata: drive %d failed\n", drive);
      
      ata_drives[drive].present = 0;
      return;
    }

    if (status & 0x08) break;
  }

  uint16_t buffer[256];

  for (int i = 0; i < 256; i++) {
    buffer[i] = i586_inw(port + 0x00);
  }

  char serial[21];
  memset(serial, 0, 21);
  memcpy(serial, buffer + 10, 21);

  for (int i = 0; i < 4; i++) {
    if (i == (int)(drive)) continue;
    
    if (!memcmp(serial, ata_drives[i].serial, 20)) {
      dbg_failf("ata: drive %d has existing serial number\n", drive);
      
      ata_drives[drive].present = 0;
      return;
    }
  }

  ata_drives[drive].size = ((uint64_t)(buffer[0x66]) << 32) | ((uint64_t)(buffer[0x65]) << 16) | buffer[0x64];
  memcpy(ata_drives[drive].serial, serial, 21);

  dbg_donef("ata: drive %d initialized\n", drive);
  dbg_infof("- size:   %d MiB\n", (uint32_t)(ata_drives[drive].size >> 11));
  dbg_infof("- serial: %s\n", serial);

  ata_drives[drive].present = 1;
  return;
}

void ata_init(void) {
  for (uint8_t i = 0; i < 4; i++) {
    ata_init_drive(i);
  }
}

static int ata_read_sector(int drive, uint64_t lba, void *buffer) {
  uint16_t port = (drive & 0x02) ? ATA_SECONDARY : ATA_PRIMARY;
  uint8_t slot = (drive & 0x01) ? 0x50 : 0x40;

  i586_outb(slot, port + 0x06);
  i586_outb(0x00, port + 0x02);
  i586_outb((uint8_t)(lba >> 24), port + 0x03);
  i586_outb((uint8_t)(lba >> 32), port + 0x04);
  i586_outb((uint8_t)(lba >> 40), port + 0x05);
  i586_outb(0x01, port + 0x02);
  i586_outb((uint8_t)(lba >>  0), port + 0x03);
  i586_outb((uint8_t)(lba >>  8), port + 0x04);
  i586_outb((uint8_t)(lba >> 16), port + 0x05);
  i586_outb(0x24, port + 0x07);

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

int ata_read(int drive, uint64_t lba, void *buffer, size_t count) {
  while (count) {
    if (!ata_read_sector(drive, lba, buffer)) return 0;
    
    lba++;
    buffer += 512;
    count--;
  }
  
  return 1;
}

static int ata_write_sector(int drive, uint64_t lba, void *buffer) {
  uint16_t port = (drive & 0x02) ? ATA_SECONDARY : ATA_PRIMARY;
  uint8_t slot = (drive & 0x01) ? 0x50 : 0x40;

  i586_outb(slot, port + 0x06);
  i586_outb(0x00, port + 0x02);
  i586_outb((uint8_t)(lba >> 24), port + 0x03);
  i586_outb((uint8_t)(lba >> 32), port + 0x04);
  i586_outb((uint8_t)(lba >> 40), port + 0x05);
  i586_outb(0x01, port + 0x02);
  i586_outb((uint8_t)(lba >>  0), port + 0x03);
  i586_outb((uint8_t)(lba >>  8), port + 0x04);
  i586_outb((uint8_t)(lba >> 16), port + 0x05);
  i586_outb(0x34, port + 0x07);

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
    i586_outw(buffer_16[i], port + 0x00);
  }

  for (int i = 0; i < 14; i++) {
    i586_inb(port + 0x07);
  }

  return 1;
}

int ata_write(int drive, uint64_t lba, void *buffer, size_t count) {
  while (count) {
    if (!ata_write_sector(drive, lba, buffer)) return 0;
    
    lba++;
    buffer += 512;
    count--;
  }
  
  return 1;
}
