#include <nop/file.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  const char *path = getenv("PATH");
  int is_set = 0;
  
  for (int i = 1; i < argc; i++) {
    if (!is_set) {
      path = argv[i];
      is_set = 1;
    } else {
      printf("%s: invalid argument(s)\n", argv[0]);
      return 0;
    }
  }
  
  int file = file_open(path);
  
  if (!file) {
    printf("%s: cannot open '%s'\n", argv[0], path);
    return 0;
  }
  
  if (file_getmode(file) & FILE_DIRECTORY) {
    char buffer[FILE_PATH_MAX];
    
    while (file_entry(file, buffer, FILE_PATH_MAX)) {
      printf("%s\n", buffer);
    }
  } else {
    char chr;
    
    while (file_read(file, &chr, 1)) {
      putchar(chr);
    }
  }
  
  file_close(file, 0);
  return 1;
}
