#include <string.h>
#include <stdio.h>

int main(int argc, const char **argv) {
  const char *entry = "intro";
  
  if (argc > 1) {
    if (argc > 2) {
      printf("help: invalid argument(s)\n");
      return 0;
    }
    
    entry = argv[1];
  }
  
  char path[256];
  strcpy(path, "0:/help/");
  
  strcat(path, entry);
  strcat(path, ".txt");
  
  FILE *file = fopen(path, "r");
  
  if (!file) {
    printf("help: cannot open '%s'\n", path);
    return 0;
  }
  
  fclose(file);
  return 1;
}
