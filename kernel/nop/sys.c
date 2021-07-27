#include <arch/i586.h>
#include <nop/sys.h>
#include <nop/dbg.h>

void sys_panic(void) {
  i586_cli();
  
  dbg_failf("kernel panic!\n");
  for (;;);
}