#include <nop/call.h>
#include <nop/file.h>
#include <nop/prog.h>
#include <nop/term.h>
#include <nop/type.h>
#include <nop/virt.h>

int $file_open(const char *path) {
  char buffer[FILE_PATH_MAX];
  
  virt_strncpy_to_phys(prog_list[prog_id - 1].table, buffer, path, FILE_PATH_MAX);
  buffer[FILE_PATH_MAX - 1] = '\0';
  
  return file_open(buffer);
}

int $file_create(const char *path) {
  char buffer[FILE_PATH_MAX];
  
  virt_strncpy_to_phys(prog_list[prog_id - 1].table, buffer, path, FILE_PATH_MAX);
  buffer[FILE_PATH_MAX - 1] = '\0';
  
  return file_create(buffer);
}

int $file_delete(const char *path) {
  char buffer[FILE_PATH_MAX];
  
  virt_strncpy_to_phys(prog_list[prog_id - 1].table, buffer, path, FILE_PATH_MAX);
  buffer[FILE_PATH_MAX - 1] = '\0';
  
  return file_delete(buffer);
}

int $file_close(int id, int save) {
  return file_close(id, save);
}

size_t $file_write(int id, void *buffer, size_t size) {
  uint32_t *table = prog_list[prog_id - 1].table;
  size_t left = size;
  
  if ((uint32_t)(buffer) & 0x0FFF) {
    size_t read = 0x1000 - ((uint32_t)(buffer) & 0x0FFF);
    if (read > left) read = left;
    
    if (file_write(id, virt_phys(table, buffer), read) < read) {
      return 0;
    }
    
    buffer += read;
    left -= read;
  }
  
  while (left) {
    size_t read = virt_cont(table, buffer) << 12;
    if (read > left) read = left;
    
    if (file_write(id, virt_phys(table, buffer), read) < read) {
      return 0;
    }
    
    buffer += read;
    left -= read;
  }
  
  return size;
}

size_t $file_read(int id, void *buffer, size_t size) {
  uint32_t *table = prog_list[prog_id - 1].table;
  size_t left = size;
  
  if ((uint32_t)(buffer) & 0x0FFF) {
    size_t read = 0x1000 - ((uint32_t)(buffer) & 0x0FFF);
    if (read > left) read = left;
    
    if (file_read(id, virt_phys(table, buffer), read) < read) {
      return 0;
    }
    
    buffer += read;
    left -= read;
  }
  
  while (left) {
    size_t read = virt_cont(table, buffer) << 12;
    if (read > left) read = left;
    
    if (file_read(id, virt_phys(table, buffer), read) < read) {
      return 0;
    }
    
    buffer += read;
    left -= read;
  }
  
  return size;
}

size_t $file_seek(int id, off_t offset, int type) {
  return file_seek(id, offset, type);
}

int $file_resize(int id, size_t size) {
  return file_resize(id, size);
}

size_t $term_write(const char *buffer, size_t size) {
  uint32_t *table = prog_list[prog_id - 1].table;
  size_t left = size;
  
  if ((uint32_t)(buffer) & 0x0FFF) {
    size_t read = 0x1000 - ((uint32_t)(buffer) & 0x0FFF);
    if (read > left) read = left;
    
    term_write(virt_phys(table, (void *)(buffer)), read);
    
    buffer += read;
    left -= read;
  }
  
  while (left) {
    size_t read = virt_cont(table, (void *)(buffer)) << 12;
    if (read > left) read = left;
    
    term_write(virt_phys(table, (void *)(buffer)), read);
    
    buffer += read;
    left -= read;
  }
  
  return size;
}

int $prog_load(const char *path, const char **argv, const char **envp, call_t *call_array, int call_count);

int $prog_kill(int id) {
  return prog_kill(id);
}

int $prog_wait(int id);

void $prog_skip(void) {
  prog_waiting = 1;
}

int $prog_alloc(size_t count) {
  return prog_alloc(prog_id, count);
}
