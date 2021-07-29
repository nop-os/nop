#include <nop/nmi.h>
#include <arch/i586.h>

void enable_nmi(void) {
  i586_outb(inb(0x70) & 0x7F, 0x70);
  i586_inb(0x71);
}

void disable_nmi(void) {
  i586_outb(inb(0x70) | 0x80, 0x70);
  i586_inb(0x71);
}
