#include <stddef.h>

#ifdef __TINYC__
typedef signed long long int64_t;
typedef signed int       int32_t;
typedef signed short     int16_t;
typedef signed char      int8_t;

typedef unsigned long long uint64_t;
typedef unsigned int       uint32_t;
typedef unsigned short     uint16_t;
typedef unsigned char      uint8_t;
#else
#include <stdint.h>
#endif

extern void nop_call(const char *name, uint32_t arg_1, uint32_t arg_2, uint32_t arg_3, uint32_t arg_4);

int _start(const char **argv, const char **envp) {
  for (;;) {
    nop_call("term_write", (uint32_t)("hello, world!\n"), 14, 0, 0);
  }
  
  return 1;
}
