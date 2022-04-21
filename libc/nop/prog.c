#include <nop/call.h>
#include <nop/file.h>
#include <nop/prog.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int prog_load(const char *path, const char **argv, const char **envp, call_t *call_array, int call_count) {
  char new_path[FILE_PATH_MAX];
  
  if (strlen(path) >= 2 && path[0] >= '0' && path[0] <= '9' && path[1] == ':') {
    strcpy(new_path, path);
  } else {
    strcpy(new_path, getenv("PATH"));
    strcat(new_path, "/");
    strcat(new_path, path);
    
    int prog = nop_call("prog_load", (uint32_t)(new_path), (uint32_t)(argv), (uint32_t)(envp), (uint32_t)(call_array), call_count);
    
    if (!prog) {
      strcpy(new_path, getenv("PROG_PATH"));
      strcat(new_path, "/");
      strcat(new_path, path);
      strcat(new_path, ".out");
    } else {
      return prog;
    }
  }
  
  return nop_call("prog_load", (uint32_t)(new_path), (uint32_t)(argv), (uint32_t)(envp), (uint32_t)(call_array), call_count);
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
