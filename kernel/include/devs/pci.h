#ifndef __DEVS_PCI_H__
#define __DEVS_PCI_H__

#include <nop/type.h>
#include <nop/conn.h>

#define PCI_VENDOR_ID   0x00
#define PCI_DEVICE_ID   0x02
#define PCI_SUBCLASS    0x0A
#define PCI_CLASS       0x0B
#define PCI_HEADER_TYPE 0x0E

extern conn_hand_t pci_hand;

const char *pci_get_class(uint8_t class);
const char *pci_get_subclass(uint8_t class, uint8_t subclass);

const char *pci_get_vendor(uint16_t vendor);
const char *pci_get_device(uint16_t device, uint16_t vendor);

uint8_t pci_read_byte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t pci_read_word(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t pci_read_dword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

void pci_init_func(uint8_t bus, uint8_t slot, uint8_t func);
void pci_init_dev(uint8_t bus, uint8_t slot);
void pci_init_bus(uint8_t bus);

void pci_init_all(void);

#endif