#include <nop/file.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  if (argc != 3) {
    printf("%s: invalid argument(s)\n", argv[0]);
    return 0;
  }
  
  char path[FILE_PATH_MAX];
  strcpy(path, argv[2]);
  
  if (path[strlen(path) - 1] == '/') {
    strcat(path, argv[1]);
  }
  
  int file = file_open(argv[1]);
  
  if (!file) {
    printf("%s: cannot open '%s'\n", argv[0], argv[1]);
    return 0;
  }
  
  int dest = file_open(path);
  
  if (dest) {
    printf("%s: '%s' already exists\n", argv[0], path);
    
    file_close(file, 0);
    file_close(dest, 0);
    
    return 0;
  }
  
  if (!file_create(path)) {
    printf("%s: cannot create '%s'\n", argv[0], path);
    
    file_close(file, 0);
    return 0;
  }
  
  dest = file_open(path);
  
  if (!dest) {
    printf("%s: cannot open '%s'\n", argv[0], path);
    
    file_close(file, 0);
    return 0;
  }
  
  size_t size = file_seek(file, 0, SEEK_END);
  
  if (!file_resize(dest, size)) {
    printf("%s: cannot resize '%s'\n", argv[0], path);
    
    file_close(file, 0);
    file_close(dest, 0);
    
    return 0;
  }
  
  file_seek(file, 0, SEEK_SET);
  file_seek(dest, 0, SEEK_SET);
  
  while (size) {
    uint8_t buffer[512];
    
    if (!file_read(file, buffer, size > 512 ? 512 : size)) {
      printf("%s: read error\n", argv[0]);
      
      file_close(file, 0);
      file_close(dest, 0);
      
      return 0;
    }
    
    if (!file_write(dest, buffer, size > 512 ? 512 : size)) {
      printf("%s: write error\n", argv[0]);
      
      file_close(file, 0);
      file_close(dest, 0);
      
      return 0;
    }
    
    size -= size > 512 ? 512 : size;
  }
  
  if (!file_setmode(dest, file_getmode(file))) {
    printf("%s: cannot set mode\n", argv[0], path);
    
    file_close(file, 0);
    file_close(dest, 0);
    
    return 0;
  }
  
  file_close(file, 0);
  
  if (!file_close(dest, 1)) {
    printf("%s: cannot save '%s'\n", argv[0], path);
    return 0;
  }
  
  if (!file_delete(argv[1])) {
    printf("%s: cannot delete '%s'\n", argv[0], argv[1]);
    return 0;
  }
  
  return 1;
}
