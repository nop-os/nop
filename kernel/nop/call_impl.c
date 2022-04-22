#include <nop/alloc.h>
#include <nop/call.h>
#include <nop/file.h>
#include <nop/prog.h>
#include <nop/term.h>
#include <nop/type.h>
#include <nop/virt.h>
#include <nop/ps2.h>

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

size_t $term_read(char *buffer, size_t size) {
  uint32_t *table = prog_list[prog_id - 1].table;
  size_t read = 0;
  
  while (size--) {
    char chr = ps2_read();
    if (!chr) break;
    
    *((char *)(virt_phys(table, buffer++))) = chr;
    read++;
  }
  
  return read;
}

int $prog_load(const char *path, const char **argv, const char **envp, call_t *call_array, int call_count) {
  uint32_t *table = prog_list[prog_id - 1].table;
  char buffer[FILE_PATH_MAX];
  
  virt_strncpy_to_phys(prog_list[prog_id - 1].table, buffer, path, FILE_PATH_MAX);
  buffer[FILE_PATH_MAX - 1] = '\0';
  
  char **new_argv = NULL;
  int argc = 0;
  
  char **new_envp = NULL;
  int envc = 0;
  
  if (argv) {
    while (*((const char **)(virt_phys(table, argv + argc)))) {
      new_argv = realloc(new_argv, (argc + 1) * sizeof(const char *));
      new_argv[argc] = virt_strdup(table, *((const char **)(virt_phys(table, argv + argc))));
      
      argc++;
    }
  }
  
  if (envp) {
    while (*((const char **)(virt_phys(table, envp + envc)))) {
      new_envp = realloc(new_envp, (envc + 1) * sizeof(const char *));
      new_envp[envc] = virt_strdup(table, *((const char **)(virt_phys(table, envp + envc))));
      
      envc++;
    }
  }
  
  new_argv = realloc(new_argv, (argc + 1) * sizeof(const char *));
  new_argv[argc] = NULL;
  
  new_envp = realloc(new_envp, (envc + 1) * sizeof(const char *));
  new_envp[envc] = NULL;
  
  call_t *new_call_array = NULL;
  
  if (call_array && call_count) {
    new_call_array = malloc(call_count * sizeof(call_t));
    virt_memcpy_to_phys(table, new_call_array, call_array, call_count * sizeof(call_t));
  }
  
  return prog_load(buffer, new_argv, new_envp, new_call_array, call_count);
}

int $prog_kill(int id) {
  return prog_kill(id);
}

int $prog_wait(int id) {
  if (!id) return 0;
  if (prog_list[id - 1].free) return 0;
  
  if (prog_list[id - 1].done) {
    return prog_list[id - 1].value;
  } else {
    prog_list[prog_id - 1].wait = id;
    prog_waiting = 1;
    
    return 0; // this *must* be overwritten by our kernel gods later on, or i'll cry :D
  }
}

void $prog_skip(void) {
  prog_waiting = 1;
}

void *$prog_alloc(size_t count) {
  if (!prog_alloc(prog_id, count)) return NULL;
  else return VIRT_NOP_USER + (((prog_list[prog_id - 1].data.size + 0x0FFF) >> 12) << 12);
}
