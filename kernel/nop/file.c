#include <nop/alloc.h>
#include <nop/term.h>
#include <nop/file.h>
#include <nop/page.h>
#include <nop/type.h>
#include <nop/fat.h>
#include <nop/ps2.h>
#include <string.h>

file_t *file_arr = NULL;
int file_cnt = 0;

int file_open(const char *path) {
  fat_node_t node;
  
  if (strlen(path) < 2) return 0;
  if (!(path[0] >= '0' && path[0] <= '9')) return 0;
  if (path[1] != ':') return 0;
  
  int part = path[0] - '0';
  
  for (int i = 0; i < file_cnt; i++) {
    if (file_arr[i].free) {
      uint32_t cluster = fat_find(part, 0, NULL, NULL, &node, path + 2);
      if (!fat_valid(cluster)) return 0;
      
      strcpy(file_arr[i].path, path);
      
      file_arr[i].size = node.size;
      file_arr[i].mode = (node.read_only * FILE_READ_ONLY) | (node.hidden * FILE_HIDDEN) | (node.system * FILE_SYSTEM) | (node.directory * FILE_DIRECTORY);
      
      file_arr[i].buffer = page_alloc((node.size + 0x0FFF) >> 12, 0);
      file_arr[i].offset = 0;
      
      fat_load_chain(part, file_arr[i].buffer, cluster);
      
      file_arr[i].free = 0;
      return i + 1;
    }
  }
  
  file_arr = realloc(file_arr, (file_cnt + 16) * sizeof(file_t));
  
  for (int i = file_cnt; i < file_cnt + 16; i++) {
    file_arr[i].free = 1;
  }
  
  file_cnt += 16;
  return file_open(path);
}

int file_create(const char *path) {
  if (strlen(path) < 2) return 0;
  if (!(path[0] >= '0' && path[0] <= '9')) return 0;
  if (path[1] != ':') return 0;
  
  uint32_t parent;
  
  int part = path[0] - '0';
  if (fat_find(part, 0, &parent, NULL, NULL, path + 2) != 0x0FFFFFFF) return 0;
  
  size_t index = fat_get_free(part, parent);
  if (index == 0xFFFFFFFF) return 0;
  
  path = strrchr(path, '/');
  if (!path) return 0;
  
  path++;
  
  fat_node_t node;
  int pos = 0;
  
  memset(node.name, ' ', 11);
  
  while (*path) {
    if (pos == 11) break;
    if (*path == '/') break;
    
    if (*path == '.') {
      if (pos < 8) {
        pos = 8;
      }
    } else if (*path >= 'a' && *path <= 'z') {
      node.name[pos++] = (*path - 'a') + 'A';
    } else {
      node.name[pos++] = *path;
    }
    
    path++;
  }
  
  node.read_only = 0;
  node.hidden = 0;
  node.system = 0;
  node.volume_id = 0;
  node.directory = 0;
  node.archive = 0;
  
  node.cluster_hi = 0;
  node.cluster_lo = 0;
  
  // TODO: set date
  
  node.size = 0;
  fat_set_node(part, parent, index, node);
  
  return 1;
}

int file_delete(const char *path) {
  if (strlen(path) < 2) return 0;
  if (!(path[0] >= '0' && path[0] <= '9')) return 0;
  if (path[1] != ':') return 0;
  
  int part = path[0] - '0';
  
  for (int i = 0; i < file_cnt; i++) {
    if (!file_arr[i].free && !strcmp(path, file_arr[i].path)) {
      term_failf("won't delete open file!\n");
      return 0;
    }
  }
  
  uint32_t parent = 0;
  size_t index = 0;
  fat_node_t node;
  
  uint32_t cluster = fat_find(part, 0, &parent, &index, &node, path + 2);
  if (cluster == 0x0FFFFFFF) return 0;
  
  fat_free_chain(part, cluster);
  
  node.name[0] = 0xE5;
  fat_set_node(part, parent, index, node);
  
  return 1;
}

int file_close(int id, int save) {
  if (id < 1 || id > file_cnt) return 0;
  else if (file_arr[id - 1].free) return 0;
  
  if (save) {
    int part = file_arr[id - 1].path[0] - '0';
    
    uint32_t parent;
    size_t index;
    fat_node_t node;
    
    uint32_t old_cluster = fat_find(part, 0, &parent, &index, &node, file_arr[id - 1].path + 2);
    
    if (old_cluster == 0x0FFFFFFF) {
      term_warnf("cannot save file: file was deleted while open\n");
      return 0;
    }
    
    fat_free_chain(part, old_cluster);
    
    uint32_t new_cluster = fat_requ_chain(part, file_arr[id - 1].size);
    
    if (!fat_edit_chain(part, file_arr[id - 1].buffer, new_cluster)) {
      term_failf("failed to edit chain!\n");
      return 0;
    }
    
    node.cluster_lo = new_cluster >>  0;
    node.cluster_hi = new_cluster >> 16;
    
    node.read_only = (file_arr[id - 1].mode & FILE_READ_ONLY) > 0;
    node.hidden = (file_arr[id - 1].mode & FILE_HIDDEN) > 0;
    node.system = (file_arr[id - 1].mode & FILE_SYSTEM) > 0;
    node.directory = (file_arr[id - 1].mode & FILE_DIRECTORY) > 0;
    
    // TODO: set date
    
    node.size = file_arr[id - 1].size;
    fat_set_node(part, parent, index, node);
  }
  
  page_free(file_arr[id - 1].buffer, (file_arr[id - 1].size + 0x0FFF) >> 12);
  file_arr[id - 1].free = 1;
  
  return 1;
}

int file_resize(int id, size_t size) {
  if (id < 1 || id > file_cnt) return 0;
  else if (file_arr[id - 1].free) return 0;
  
  if (file_arr[id - 1].offset > size) {
    file_arr[id - 1].offset = size;
  }
  
  void *buffer = page_alloc((size + 0x0FFF) >> 12, 0);
  memcpy(buffer, file_arr[id - 1].buffer, size < file_arr[id - 1].size ? size : file_arr[id - 1].size);
  
  page_free(file_arr[id - 1].buffer, (file_arr[id - 1].size + 0x0FFF) >> 12);
  
  file_arr[id - 1].buffer = buffer;
  file_arr[id - 1].size = size;
  
  return 1;
}

int file_getmode(int id) {
  if (id < 1 || id > file_cnt) return 0;
  else if (file_arr[id - 1].free) return 0;
  
  return file_arr[id - 1].mode;
}

int file_setmode(int id, int mode) {
  if (id < 1 || id > file_cnt) return 0;
  else if (file_arr[id - 1].free) return 0;
  
  file_arr[id - 1].mode = mode;
  return 1;
}

size_t file_seek(int id, off_t offset, int type) {
  if (id < 1 || id > file_cnt) return 0;
  else if (file_arr[id - 1].free) return 0;
  
  if (type == SEEK_CUR) {
    file_arr[id - 1].offset += offset;
  } else if (type == SEEK_END) {
    file_arr[id - 1].offset = file_arr[id - 1].size + offset;
  } else {
    file_arr[id - 1].offset = offset;
  }
  
  if (file_arr[id - 1].offset > file_arr[id - 1].size) {
    file_arr[id - 1].offset = file_arr[id - 1].size;
  }
  
  return file_arr[id - 1].offset;
}

size_t file_tell(int id) {
  if (id < 1 || id > file_cnt) return 0;
  else if (file_arr[id - 1].free) return 0;
  
  return file_arr[id - 1].offset;
}

int file_eof(int id) {
  if (id < 0 || id > file_cnt) {
    return 0;
  } else if (!id) {
    return 1;
  } else if (file_arr[id - 1].free) {
    return 0;
  }
  
  return (file_arr[id - 1].offset == file_arr[id - 1].size);
}

size_t file_read(int id, void *buffer, size_t size) {
  if (id < 0 || id > file_cnt) {
    return 0;
  } else if (!id) {
    uint16_t value;
    size_t left = size;
    
    while (left && (value = ps2_read())) {
      *((uint8_t *)(buffer)) = (uint8_t)(value);
      left--;
    }
    
    return size - left;
  } else if (file_arr[id - 1].free) {
    return 0;
  }
  
  if (size > file_arr[id - 1].size - file_arr[id - 1].offset) {
    size = file_arr[id - 1].size - file_arr[id - 1].offset;
  }
  
  memcpy(buffer, file_arr[id - 1].buffer + file_arr[id - 1].offset, size);
  file_arr[id - 1].offset += size;
  
  return size;
}

size_t file_write(int id, void *buffer, size_t size) {
  if (id < 0 || id > file_cnt) {
    return 0;
  } else if (!id) {
    for (int i = 0; i < size; i++) {
      term_putchr(((char *)(buffer))[i]);
    }
    
    return size;
  } else if (file_arr[id - 1].free) {
    return 0;
  }
  
  if (size > file_arr[id - 1].size - file_arr[id - 1].offset) {
    // if (!file_size(id, file_arr[id - 1].offset + size)) return 0;
    size = file_arr[id - 1].size - file_arr[id - 1].offset;
  }
  
  memcpy(file_arr[id - 1].buffer + file_arr[id - 1].offset, buffer, size);
  file_arr[id - 1].offset += size;
  
  return size;
}

int file_entry(int id, char *buffer, size_t size) {
  fat_node_t node;
  
  for (;;) {
    if (!file_read(id, &node, sizeof(node))) return 0;
    if (node.name[0] && node.name[0] != 0xE5 && node.name[0] != '.') break;
  }
  
  int offset = 0;
  
  for (int i = 0; i < 8; i++) {
    if (node.name[i] == ' ') break;
    if (offset >= size - 1) break;
    
    if (node.name[i] >= 'A' && node.name[i] <= 'Z') {
      buffer[offset++] = (node.name[i] - 'A') + 'a';
    } else {
      buffer[offset++] = node.name[i];
    }
  }
  
  if (node.name[8] != ' ' && offset < size - 1) {
    buffer[offset++] = '.';
    
    for (int i = 8; i < 11; i++) {
      if (node.name[i] == ' ') break;
      if (offset >= size - 1) break;
      
      if (node.name[i] >= 'A' && node.name[i] <= 'Z') {
        buffer[offset++] = (node.name[i] - 'A') + 'a';
      } else {
        buffer[offset++] = node.name[i];
      }
    }
  }
  
  if (offset < size - 1) {
    buffer[offset] = '\0';
  }
  
  return 1;
}
