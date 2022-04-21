#include <nop/call.h>
#include <nop/file.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int file_open(const char *path) {
  char new_path[FILE_PATH_MAX];
  
  if (strlen(path) >= 2 && path[0] >= '0' && path[0] <= '9' && path[1] == ':') {
    strcpy(new_path, path);
  } else {
    strcpy(new_path, getenv("PATH"));
    strcat(new_path, "/");
    strcat(new_path, path);
  }
  
  return nop_call("file_open", (uint32_t)(new_path), 0, 0, 0, 0);
}

int file_create(const char *path) {
  char new_path[FILE_PATH_MAX];
  
  if (strlen(path) >= 2 && path[0] >= '0' && path[0] <= '9' && path[1] == ':') {
    strcpy(new_path, path);
  } else {
    strcpy(new_path, getenv("PATH"));
    strcat(new_path, "/");
    strcat(new_path, path);
  }
  
  return nop_call("file_create", (uint32_t)(new_path), 0, 0, 0, 0);
}

int file_delete(const char *path) {
  char new_path[FILE_PATH_MAX];
  
  if (strlen(path) >= 2 && path[0] >= '0' && path[0] <= '9' && path[1] == ':') {
    strcpy(new_path, path);
  } else {
    strcpy(new_path, getenv("PATH"));
    strcat(new_path, "/");
    strcat(new_path, path);
  }
  
  return nop_call("file_delete", (uint32_t)(new_path), 0, 0, 0, 0);
}

int file_close(int id, int save) {
  return nop_call("file_close", id, save, 0, 0, 0);
}

size_t file_seek(int id, off_t offset, int type) {
  return nop_call("file_seek", id, offset, type, 0, 0);
}

int file_resize(int id, size_t size) {
  return nop_call("file_resize", id, size, 0, 0, 0);
}

size_t file_read(int id, void *buffer, size_t size) {
  return nop_call("file_read", id, (uint32_t)(buffer), size, 0, 0);
}

size_t file_write(int id, const void *buffer, size_t size) {
  return nop_call("file_write", id, (uint32_t)(buffer), size, 0, 0);
}
