#include <nop/call.h>
#include <stdint.h>
#include <string.h>

int main(int argc, const char **argv) {
  const char *text = "hey, i'm writing this from userspace!\n";
  
  nop_call("term_write", (uint32_t)(text), strlen(text), 0, 0);
  return 1;
}
