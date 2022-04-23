#include <nop/file.h>
#include <stdlib.h>
#include <string.h>
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
      char full_path[FILE_PATH_MAX];
      
      strcpy(full_path, path);
      strcat(full_path, "/");
      strcat(full_path, buffer);
      
      int child = file_open(full_path);
      
      if (child) {
        int mode = file_getmode(child);
        
        size_t size = file_seek(child, 0, SEEK_END);
        file_seek(child, 0, SEEK_SET);
        
        printf("[%c%c%c%c] [%-10d] %s\n",
          (mode & FILE_READ_ONLY) ? 'r' : '-',
          (mode & FILE_HIDDEN) ? 'h' : '-',
          (mode & FILE_SYSTEM) ? 's' : '-',
          (mode & FILE_DIRECTORY) ? 'd' : '-',
        size, buffer);
        
        file_close(child, 0);
      }
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
