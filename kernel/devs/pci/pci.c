#include <arch/i586.h>
#include <devs/pci.h>
#include <nop/type.h>
#include <nop/conn.h>
#include <nop/dbg.h>

conn_hand_t pci_hand;

uint8_t pci_read_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
  uint32_t addr = 0x80000000;
  addr |= (uint32_t)(bus) << 16;
  addr |= (uint32_t)(slot) << 11;
  addr |= (uint32_t)(func) << 8;
  addr |= offset & 0xFC;

  i586_outd(addr, 0x0CF8);
  return (uint8_t)(i586_ind(0x0CFC) >> ((offset & 3) << 3));
}

uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
  uint32_t addr = 0x80000000;
  addr |= (uint32_t)(bus) << 16;
  addr |= (uint32_t)(slot) << 11;
  addr |= (uint32_t)(func) << 8;
  addr |= offset & 0xFC;

  i586_outd(addr, 0x0CF8);
  return (uint16_t)(i586_ind(0x0CFC) >> ((offset & 2) << 3));
}

uint32_t pci_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
  uint32_t addr = 0x80000000;
  addr |= (uint32_t)(bus) << 16;
  addr |= (uint32_t)(slot) << 11;
  addr |= (uint32_t)(func) << 8;
  addr |= offset & 0xFC;

  i586_outd(addr, 0x0CF8);
  return i586_ind(0x0CFC);
}

void pci_init_func(uint8_t bus, uint8_t slot, uint8_t func) {
  uint16_t device = pci_read_word(bus, slot, func, PCI_DEVICE_ID);
  uint16_t vendor = pci_read_word(bus, slot, func, PCI_VENDOR_ID);

  uint8_t subclass = pci_read_byte(bus, slot, func, PCI_SUBCLASS);
  uint8_t class = pci_read_byte(bus, slot, func, PCI_CLASS);

  dbg_infof("pci: (%02X:%02X:%02X)\n", bus, slot, func);
  dbg_infof("- vendor:   (%04X) %s\n", vendor, pci_get_vendor(vendor));
  dbg_infof("- device:   (%04X) %s\n", device, pci_get_device(device, vendor));
  dbg_infof("- class:    (%04X) %s\n", class, pci_get_class(class));
  dbg_infof("- subclass: (%04X) %s\n", subclass, pci_get_subclass(class, subclass));
}

void pci_init_dev(uint8_t bus, uint8_t slot) {
  if (pci_read_word(bus, slot, 0, PCI_VENDOR_ID) == 0xFFFF) return;
  pci_init_func(bus, slot, 0);

  if (!(pci_read_byte(bus, slot, 0, PCI_HEADER_TYPE) & 0x80)) return;

  for (uint8_t i = 1; i < 8; i++) {
    if (pci_read_word(bus, slot, i, PCI_VENDOR_ID) == 0xFFFF) continue;
    pci_init_func(bus, slot, i);
  }
}

void pci_init_bus(uint8_t bus) {
  for (uint8_t i = 0; i < 32; i++) {
    pci_init_dev(bus, i);
  }
}

void pci_init_all(void) {
  pci_init_bus(0);

  if (!(pci_read_byte(0, 0, 0, PCI_HEADER_TYPE) & 0x80)) return;

  for (uint8_t i = 1; i < 8; i++) {
    if (pci_read_word(0, 0, i, PCI_VENDOR_ID) == 0xFFFF) continue;
    pci_init_bus(i);
  }
}