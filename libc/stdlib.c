#include <nop/alloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const char **environ = NULL;

jmp_buf _start_buf;
int _start_ret;

void *malloc(size_t size) {
  return nop_alloc(size);
}

void free(void *ptr) {
  nop_free(ptr);
}

void *calloc(size_t count, size_t size) {
  void *ptr = malloc(count * size);
  if (!ptr) return NULL;
  
  memset(ptr, 0, count * size);
  return ptr;
}

void *realloc(void *ptr, size_t size) {
  return nop_realloc(ptr, size);
}

int setenv(const char *name, const char *value, int overwrite) {
  return 0;
}

int unsetenv(const char *name) {
  return 0;
}

const char *getenv(const char *name) {
  for (int i = 0; environ[i]; i++) {
    char env_name[32] = {0};
    
    for (int j = 0; j < 31 && environ[i][j] && environ[i][j] != '='; j++) {
      env_name[j] = environ[i][j];
    }
    
    if (!strcmp(name, env_name)) {
      return environ[i] + strlen(env_name) + 1;
    }
  }
  
  return NULL;
}

noreturn void exit(int value) {
  _start_ret = value;
  longjmp(_start_buf, 1);
}
