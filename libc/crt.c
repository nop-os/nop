#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char **argv, const char **envp);

int _start(const char **argv, const char **envp) {
  int argc = 0;
  while (argv[argc]) argc++;
  
  int envc = 0;
  while (envp[envc]) envc++;
  
  environ = calloc(envc, sizeof(const char *));
  
  for (int i = 0; i < envc; i++) {
    environ[i] = strdup(envp[i]);
  }
  
  if (setjmp(_start_buf)) {
    return _start_ret;
  } else {
    return main(argc, argv, envp);
  }
}
