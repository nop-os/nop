#include <string.h>
#include <nop/device.h>

unsigned int cursor = 0;

void puts(const char *str) {
  unsigned short *txt_buf = (unsigned short *)(0xB8000 + cursor);

  while (*str) {
    txt_buf[cursor] = 0x0800 | *str;
    cursor++; str++;
  }
}

void nop_start(void) {
  device_init();

  memset(0xB8000, 0x00, 80 * 25 * 2);
  puts("Hello, world!");
}
