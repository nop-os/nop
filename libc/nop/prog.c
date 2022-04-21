#include <nop/call.h>
#include <nop/prog.h>
#include <stdint.h>

int prog_load(const char *path, const char **argv, const char **envp, call_t *call_array, int call_count) {
  // TODO: translate paths with $PATH
  return nop_call("prog_load", (uint32_t)(path), (uint32_t)(argv), (uint32_t)(envp), (uint32_t)(call_array), call_count);
}

int prog_kill(int id) {
  return nop_call("prog_kill", id, 0, 0, 0, 0);
}

int prog_wait(int id) {
  return nop_call("prog_wait", id, 0, 0, 0, 0);
}

void prog_skip(void) {
  nop_call("prog_skip", 0, 0, 0, 0, 0);
}

int prog_alloc(size_t count) {
  return nop_call("prog_alloc", count, 0, 0, 0, 0);
}
