#include <nop/sys.h>
#include <nop/dbg.h>

void sys_panic(void) {
  dbg_failf("kernel panic!\n");
  for (;;);
}