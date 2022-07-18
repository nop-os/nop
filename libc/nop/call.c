#include <nop/call.h>
#include <stdint.h>

size_t call_read(void *dest, const void *src, size_t size) {
  return nop_call("call_read", (uint32_t)(dest), (uint32_t)(src), size, 0, 0);
}

size_t call_write(void *dest, const void *src, size_t size) {
  return nop_call("call_write", (uint32_t)(dest), (uint32_t)(src), size, 0, 0);
}

int call_getid(void) {
  return nop_call("call_getid", 0, 0, 0, 0, 0);
}
