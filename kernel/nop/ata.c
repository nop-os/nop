#include <arch/i586.h>
#include <nop/alloc.h>
#include <nop/term.h>
#include <nop/real.h>
#include <nop/time.h>
#include <nop/type.h>
#include <nop/ata.h>
#include <string.h>

ata_t ata_drives[9];

static uint16_t ata_get_port(uint8_t index) {
  if (index == 0) return ATA_PRIMARY;
  else if (index == 1) return ATA_SECONDARY;
  else if (index == 2) return ATA_TERTIARY;
  else return ATA_QUATERNARY;
}

static uint16_t ata_get_ctrl(uint8_t index) {
  if (index == 0) return ATA_CTRL_PRIMARY;
  else if (index == 1) return ATA_CTRL_SECONDARY;
  else if (index == 2) return ATA_CTRL_TERTIARY;
  else return ATA_CTRL_QUATERNARY;
}

static void ata_init_drive(uint8_t drive) {
  if (drive >= 0x08) {
    uint16_t buffer[256];
    
    uint8_t id = *((uint8_t *)(0x00007010));
    
    if (!real_read(id, 0, buffer)) {
      ata_drives[drive].present = 0;
      return;
    }
    
    if (buffer[255] != 0xAA55) {
      ata_drives[drive].present = 0;
      return;
    }
    
    strcpy(ata_drives[drive].serial, "DONTREADWITHBIOSKIDS");
    
    ata_drives[drive].present = 1;
    ata_drives[drive].size = id;
    
    term_donef("drive %d initialized(0x%02X)\n", drive, id);
    return;
  }
  
  uint16_t port = ata_get_port(drive >> 1);
  uint8_t slot = (drive & 0x01) ? 0xB0 : 0xA0;

  uint8_t status = i586_inb(port + 0x07);

  if (status == 0xFF) {
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
      ata_drives[drive].present = 0;
      return;
    }

    if (!(status & 0x80)) break;
  }

  for (;;) {
    status = i586_inb(port + 0x07);

    if (status & 0x01) {
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

  for (int i = 0; i < 8; i++) {
    if (i == (int)(drive)) continue;
    
    if (!memcmp(serial, ata_drives[i].serial, 20)) {
      ata_drives[drive].present = 0;
      return;
    }
  }

  ata_drives[drive].size = ((uint64_t)(buffer[0x66]) << 32) | ((uint64_t)(buffer[0x65]) << 16) | buffer[0x64];
  memcpy(ata_drives[drive].serial, serial, 21);

  term_donef("drive %d initialized\n", drive);
  term_infof("- size:   %d MiB\n", (uint32_t)(ata_drives[drive].size >> 11));
  term_infof("- serial: %s\n", serial);

  ata_drives[drive].present = 1;
  return;
}

void ata_init(void) {
  for (uint8_t i = 0; i < 9; i++) {
    ata_init_drive(i);
  }
}

void ata_reset(int drive) {
  uint16_t port = ata_get_port(drive >> 1);
  uint16_t ctrl = ata_get_ctrl(drive >> 1);
  
  i586_outb(0x06, ctrl + 0x00);
  i586_wait();
  
  i586_outb(0x02, ctrl + 0x00);
  i586_wait();
}

static int ata_read_sector(int drive, uint64_t lba, void *buffer) {
  if (drive >= 0x08) {
    return real_read(ata_drives[drive].size, lba, buffer);
  }
  
  uint16_t *buffer_16 = buffer;
  
  uint16_t port = ata_get_port(drive >> 1);
  uint16_t ctrl = ata_get_ctrl(drive >> 1);
  
  uint8_t slot = (drive & 0x01) ? 0x50 : 0x40;
  i586_cli();
  
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
  
  for (int i = 0; i < 4; i++) {
    uint8_t status = i586_inb(port + 0x07);
    
    if (status & 0x80) continue;
    if (status & 0x08) goto read_ready;
  }
  
  for (;;) {
    uint8_t status = i586_inb(port + 0x07);
    
    if (status & 0x80) continue;
    if (status & 0x21) return 0;
    
    break;
  }
  
read_ready:
  for (int i = 0; i < 256; i++) {
    buffer_16[i] = i586_inw(port + 0x00);
  }
  
  for (int i = 0; i < 4; i++) {
    i586_inb(port + 0x07);
  }
  
  i586_sti();
  return 1;
}

int ata_read(int drive, uint64_t lba, void *buffer, size_t count) {
  while (count) {
    if (lba >= ata_drives[drive].size) break;
    int success = 0;
    
    for (int i = 0; i < 256; i++) {
      if (ata_read_sector(drive, lba, buffer)) {
        success = 1;
        break;
      }
    }
    
    if (!success) {
      term_warnf("read failed on drive %d\n", drive);
      return 0;
    }
    
    lba++;
    buffer += 512;
    count--;
  }
  
  return 1;
}

static int ata_write_sector(int drive, uint64_t lba, void *buffer) {
  if (drive >= 0x08) {
    return real_write(ata_drives[drive].size, lba, buffer);
  }
  
  uint16_t *buffer_16 = buffer;
  
  uint16_t port = ata_get_port(drive >> 1);
  uint16_t ctrl = ata_get_ctrl(drive >> 1);
  
  uint8_t slot = (drive & 0x01) ? 0x50 : 0x40;
  i586_cli();
  
  i586_outb(slot, port + 0x06);
  
  i586_outb(0x00, port + 0x02);
  i586_outb((uint8_t)(lba >> 24), port + 0x03);
  i586_outb((uint8_t)(lba >> 32), port + 0x04);
  i586_outb((uint8_t)(lba >> 40), port + 0x05);
  
  i586_outb(0x01, port + 0x02);
  i586_outb((uint8_t)(lba >>  0), port + 0x03);
  i586_outb((uint8_t)(lba >>  8), port + 0x04);
  i586_outb((uint8_t)(lba >> 16), port + 0x05);
  i586_wait();
  
  i586_outb(0x34, port + 0x07);
  
  for (int i = 0; i < 4; i++) {
    uint8_t status = i586_inb(port + 0x07);
    
    if (status & 0x80) continue;
    if (status & 0x08) goto write_ready;
  }
  
  for (;;) {
    uint8_t status = i586_inb(port + 0x07);
    
    if (status & 0x80) continue;
    if (status & 0x21) return 0;
    
    break;
  }
  
write_ready:
  for (int i = 0; i < 256; i++) {
    i586_outw(buffer_16[i], port + 0x00);
    i586_wait();
  }
  
  for (int i = 0; i < 4; i++) {
    i586_inb(port + 0x07);
  }
  
  i586_outb(0xEA, port + 0x07); // should be 0xE7???
  
  i586_sti();
  return 1;
}

int ata_write(int drive, uint64_t lba, void *buffer, size_t count) {
  while (count) {
    if (lba >= ata_drives[drive].size) break;
    int success = 0;
    
    for (int i = 0; i < 256; i++) {
      if (ata_write_sector(drive, lba, buffer)) {
        success = 1;
        break;
      }
    }
    
    if (!success) {
      term_warnf("read failed on drive %d\n", drive);
      return 0;
    }
    
    lba++;
    buffer += 512;
    count--;
  }
  
  return 1;
}
