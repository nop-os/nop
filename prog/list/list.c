#include <nop/file.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  const char *path = getenv("PATH");
  int is_set = 0;
  
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      // ...
    } else if (!is_set) {
      path = argv[i];
      is_set = 1;
    } else {
      printf("%s: invalid argument(s)\n", argv[0]);
      return 0;
    }
  }
  
  FILE *dir = fopen(path, "r");
  
  if (!dir) {
    printf("%s: cannot open '%s'\n", argv[0], path);
    return 0;
  }
  
  char buffer[FILE_PATH_MAX];
  
  while (file_entry(dir->file, buffer, FILE_PATH_MAX)) {
    printf("%s\n", buffer);
  }
  
  fclose(dir);
  return 1;
}
