#include <nop/call.h>
#include <nop/term.h>
#include <stdint.h>

size_t term_write(const char *buffer, size_t size) {
  return nop_call("term_write", (uint32_t)(buffer), size, 0, 0, 0);
}

size_t term_read(char *buffer, size_t size) {
  return nop_call("term_read", (uint32_t)(buffer), size, 0, 0, 0);
}

int term_getmode(void) {
  return nop_call("term_getmode", 0, 0, 0, 0, 0);
}

int term_setmode(int mode) {
  return nop_call("term_setmode", mode, 0, 0, 0, 0);
}

uint32_t term_getsize(void) {
  return nop_call("term_getsize", 0, 0, 0, 0, 0);
}
