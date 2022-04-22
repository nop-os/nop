#include <nop/alloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char **environ = NULL;

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
  int envc = 0;
  
  for (int i = 0; environ[i]; i++) {
    char env_name[32] = {0};
    
    for (int j = 0; j < 31 && environ[i][j] && environ[i][j] != '='; j++) {
      env_name[j] = environ[i][j];
    }
    
    if (!strcmp(name, env_name)) {
      if (overwrite) {
        environ[i] = realloc(environ[i], strlen(env_name) + 1 + strlen(value) + 1);
        strcpy(environ[i] + strlen(env_name) + 1, value);
      }
      
      return 0;
    }
    
    envc++;
  }
  
  environ = realloc(environ, (envc + 2) * sizeof(const char *));
  
  environ[envc + 0] = malloc(strlen(name) + 1 + strlen(value) + 1);
  environ[envc + 1] = NULL;
  
  strcpy(environ[envc], name);
  strcat(environ[envc], "=");
  strcat(environ[envc], value);
  
  return 0;
}

int unsetenv(const char *name) {
  return setenv(name, "", 1);
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
