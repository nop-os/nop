#include <nop/file.h>
#include <config.h>
#include <string.h>

static int cfg_is_space(char chr) {
  return (chr == ' ') || (chr == '\n') || (chr == '\t') || (chr == ':') || (chr == ',');
}

void cfg_rewind(int file) {
  file_seek(file, 0, SEEK_SET);
}

int cfg_next_str(int file, char *buffer) {
  int in_string = 0;
  char chr = '\0';
  
  for (;;) {
    if (!file_read(file, &chr, 1)) return 0;
    
    if (chr == '#') {
      for (;;) {
        if (!file_read(file, &chr, 1)) return 0;
        else if (chr == '\n') break;
      }
    } else if (!cfg_is_space(chr)) {
      break;
    }
  }
  
  if (chr == '[' || chr == ']') {
    buffer[0] = chr;
    buffer[1] = '\0';
    
    return 1;
  }
  
  for (;;) {
    if (cfg_is_space(chr) && !in_string) break;
    
    if (chr == '\\') {
      if (!file_read(file, &chr, 1)) break;
      *(buffer++) = chr;
      
      if (!file_read(file, &chr, 1)) break;
      continue;
    }
    
    *(buffer++) = chr;
    
    if (chr == '"') {
      in_string = !in_string;
      if (!in_string) break;
    }
    
    if (!file_read(file, &chr, 1)) break;
  }
  
  *buffer = '\0';
  return 1;
}

int cfg_read_str(int file, const char *name, char *buffer) {
  cfg_rewind(file);
  
  while (cfg_next_str(file, buffer)) {
    if (!strcmp(buffer, name)) {
      char temp[FILE_PATH_MAX + 32];
      if (!cfg_next_str(file, temp)) return 0;
      
      for (int i = temp[0] == '"'; temp[i] && temp[i] != '"'; i++) {
        *(buffer++) = temp[i];
      }
      
      *buffer = '\0';
      return 1;
    }
  }
  
  return 0;
}

int cfg_read_int(int file, const char *name, int *value) {
  char buffer[FILE_PATH_MAX + 32];
  if (!cfg_read_str(file, name, buffer)) return 0;
  
  // TODO: strtol
  return 1;
}

int cfg_read_arr_len(int file, const char *name) {
  cfg_rewind(file);
  char buffer[FILE_PATH_MAX + 32];
  
  while (cfg_next_str(file, buffer)) {
    if (!strcmp(buffer, name)) {
      if (!cfg_next_str(file, buffer)) return 0;
      
      int length = 0;
      
      for (;;) {
        if (!cfg_next_str(file, buffer)) return 0;
        if (!strcmp(buffer, "]")) break;
        
        length++;
      }
      
      return length;
    }
  }
  
  return 0;
}

int cfg_read_arr_str(int file, const char *name, int index, char *buffer) {
  cfg_rewind(file);
  
  while (cfg_next_str(file, buffer)) {
    if (!strcmp(buffer, name)) {
      for (int i = 0; i < index + 1; i++) {
        if (!cfg_next_str(file, buffer)) return 0;
      }
      
      char temp[FILE_PATH_MAX + 32];
      if (!cfg_next_str(file, temp)) return 0;
      
      for (int i = temp[0] == '"'; temp[i] && temp[i] != '"'; i++) {
        *(buffer++) = temp[i];
      }
      
      *buffer = '\0';
      return 1;
    }
  }
  
  return 0;
}

int cfg_read_arr_int(int file, const char *name, int index, int *value) {
  char buffer[FILE_PATH_MAX + 32];
  if (!cfg_read_arr_str(file, name, index, buffer)) return 0;
  
  // TODO: strtol
  return 1;
}
