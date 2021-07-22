#include <devs/pci.h>
#include <nop/type.h>

const char *pci_get_class(uint8_t class) {
  const char *array[] = {
    "Unclassified",           "Mass Storage Controller",
    "Network Controller",     "Display Controller",
    "Multimedia Controller",  "Memory Controller",
    "Bridge Device",          "Simple Communication Controller",
    "Base System Peripheral", "Input Device Controller",
    "Docking Station",        "Processor",
    "Serial Bus Controller",  "Wireless Controller",
    "Intelligent Controller", "Satellite Communication Controller",
    "Encryption Controller",  "Signal Processing Controller",
    "Processing Accelerator", "Non Essential Instrumentation"
  };

  if (class >= 0x14) class = 0x00;
  return array[class];
}

const char *pci_get_subclass(uint8_t class, uint8_t subclass) {
  switch (class) {
    case 0x00:
      return "Unknown";

    case 0x01:
      switch (subclass) {
        case 0x00:
          return "SCSI Storage Controller";
          
        case 0x01:
          return "IDE Interface";

        case 0x02:
          return "Floppy Disk Controller";

        case 0x03:
          return "IPI Bus Controller";

        case 0x04:
          return "RAID Bus Controller";

        case 0x05:
          return "ATA Controller";

        case 0x06:
          return "SATA Controller";

        case 0x07:
          return "Serial Attached SCSI Controller";

        case 0x08:
          return "Non-Volatile Memory Controller";
      }

      return "Mass Storage Controller";

    case 0x02:
      switch (subclass) {
        case 0x00:
          return "Ethernet Controller";

        case 0x01:
          return "Token Ring Network Controller";

        case 0x02:
          return "FDDI Network Controller";

        case 0x03:
          return "ATM Network Controller";

        case 0x04:
          return "ISDN Controller";

        case 0x05:
          return "WorldFip Controller";

        case 0x06:
          return "PICMG Controller";

        case 0x07:
          return "Infiniband Controller";

        case 0x08:
          return "Fabric Controller";
      }

      return "Network Controller";

    case 0x03:
      switch (subclass) {
        case 0x00:
          return "VGA Compatible Controller";

        case 0x01:
          return "XGA Compatible Controller";

        case 0x02:
          return "3D Controller";
      }

      return "Display Controller";

    case 0x04:
      switch (subclass) {
        case 0x00:
          return "Multimedia Video Controller";

        case 0x01:
          return "Multimedia Audio Controller";

        case 0x02:
          return "Computer Telephony Device";

        case 0x03:
          return "Audio Device";
      }

      return "Multimedia Controller";

    case 0x05:
      switch (subclass) {
        case 0x00:
          return "RAM Memory";

        case 0x01:
          return "FLASH Memory";
      }

      return "Memory Controller";

    case 0x06:
      switch (subclass) {
        case 0x00:
          return "Host Bridge";

        case 0x01:
          return "ISA Bridge";

        case 0x02:
          return "EISA Bridge";

        case 0x03:
          return "MicroChannel Bridge";

        case 0x04:
          return "PCI Bridge";

        case 0x05:
          return "PCMCIA Bridge";

        case 0x06:
          return "NuBus Bridge";

        case 0x07:
          return "CardBus Bridge";

        case 0x08:
          return "RACEway Bridge";

        case 0x09:
          return "Semi-Transparent PCI To PCI Bridge";

        case 0x0A:
          return "InfiniBand To PCI Host Bridge";
      }

      return "Bridge";

    case 0x07:
      switch (subclass) {
        case 0x00:
          return "Serial Controller";

        case 0x01:
          return "Parallel Controller";

        case 0x02:
          return "Multiport Serial Controller";

        case 0x03:
          return "Modem";

        case 0x04:
          return "GPIB Controller";

        case 0x05:
          return "Smart Card Controller";
      }

      return "Communication Controller";

    case 0x08:
      switch (subclass) {
        case 0x00:
          return "PIC";

        case 0x01:
          return "DMA Controller";

        case 0x02:
          return "Timer";

        case 0x03:
          return "RTC";

        case 0x04:
          return "PCI Hot-Plug Controller";

        case 0x05:
          return "SD Host Controller";

        case 0x06:
          return "IOMMU";
      }

      return "System Peripheral";

    case 0x09:
      switch (subclass) {
        case 0x00:
          return "Keyboard Controller";

        case 0x01:
          return "Digitizer Pen";

        case 0x02:
          return "Mouse Controller";

        case 0x03:
          return "Scanner Controller";

        case 0x04:
          return "Gameport Controller";
      }

      return "Input Device Controller";

    case 0x0A:
      switch (subclass) {
        case 0x00:
          return "Generic Docking Station";
      }

      return "Docking Station";

    case 0x0B:
      switch (subclass) {
        case 0x00:
          return "80386";

        case 0x01:
          return "80486";

        case 0x02:
          return "Pentium";

        case 0x0A:
          return "Alpha";

        case 0x14:
          return "PowerPC";

        case 0x1E:
          return "MIPS";

        case 0x28:
          return "Coprocessor";
      }

      return "Processor";

    case 0x0C:
      switch (subclass) {
        case 0x00:
          return "FireWire (IEEE 1394)";

        case 0x01:
          return "ACCESS Bus";

        case 0x02:
          return "SSA";

        case 0x03:
          return "USB Controller";

        case 0x04:
          return "Fibre Channel";

        case 0x05:
          return "SMBus";

        case 0x06:
          return "InfiniBand";

        case 0x07:
          return "IPMI Interface";

        case 0x08:
          return "SERCOS Interface";

        case 0x09:
          return "CANBUS";
      }

      return "Serial Bus Controller";

    case 0x0D:
      switch (subclass) {
        case 0x00:
          return "IRDA Controller";

        case 0x01:
          return "Consumer IR Controller";

        case 0x0A:
          return "RF Controller";

        case 0x0B:
          return "Bluetooth";

        case 0x0C:
          return "Broadband";

        case 0x14:
          return "802.1a Controller";

        case 0x15:
          return "802.1b Controller";
      }

      return "Wireless Controller";

    case 0x0E:
      switch (subclass) {
        case 0x00:
          return "I2O";
      }

      return "Intelligent Controller";
  }

  return "Unknown";
}

const char *pci_get_vendor(uint16_t vendor) {
  switch (vendor) {
    case 0x1002:
      return "Advanced Micro Devices (AMD) Inc.";

    case 0x1014:
      return "IBM";

    case 0x10DE:
      return "NVIDIA";

    case 0x10EC:
      return "Realtek Semiconductor Co., Ltd.";

    case 0x1234:
      return "Technical Corp.";

    case 0x1414:
      return "Microsoft Corporation";

    case 0x1A58:
      return "Razer USA Ltd.";

    case 0x1AE0:
      return "Google, Inc.";

    case 0x1AF4:
      return "Red Hat";

    case 0x168C:
      return "Qualcomm Atheros";

    case 0x8086:
      return "Intel Corporation";

    case 0x80EE:
      return "Oracle Corporation";
  }

  return "Unknown";
}

const char *pci_get_device(uint16_t device, uint16_t vendor) {
  switch (vendor) {
    case 0x10EC:
      switch (device) {
        case 0x8139:
          return "RTL-8100/8101L/8139 PCI Fast Ethernet Adapter";
      }

      break;

    case 0x1234:
      switch (device) {
        case 0x1111:
          return "QEMU Virtual Video Controller";
      }

      break;

    case 0x8086:
      switch (device) {
        case 0x100E:
          return "Gigabit Ethernet Controller";

        case 0x153A:
          return "Ethernet Connection I217-LM";

        case 0x10EA:
          return "Gigabit Network Connection";
      }

      break;

    case 0x80EE:
      switch (device) {
        case 0xBEEF:
          return "VirtualBox Graphics Adapter";

        case 0xCAFE:
          return "VirtualBox Guest Service";
      }
      
      break;
  }

  return "Unknown";
}