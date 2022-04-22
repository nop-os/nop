#ifndef __NOP_FILE_H__
#define __NOP_FILE_H__

#include <nop/type.h>
#include <nop/fat.h>

#define FILE_PATH_MAX FAT_PATH_MAX

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define FILE_READ_ONLY (1 << 0)
#define FILE_HIDDEN    (1 << 1)
#define FILE_SYSTEM    (1 << 2)
#define FILE_DIRECTORY (1 << 3)

typedef struct file_t file_t;

struct file_t {
  int free;
  
  char path[FILE_PATH_MAX];
  int mode;
  
  void *buffer;
  size_t size, offset;
};

extern file_t *file_arr;
extern int file_cnt;

int file_open(const char *path);
int file_create(const char *path);
int file_delete(const char *path);
int file_close(int id, int save);
int file_getmode(int id);
int file_setmode(int id, int mode);

size_t  file_seek(int id, off_t offset, int type);
int     file_resize(int id, size_t size);
size_t  file_tell(int id);
int     file_eof(int id);

size_t file_read(int id, void *buffer, size_t size);
size_t file_write(int id, void *buffer, size_t size);

int file_entry(int id, char *buffer, size_t size);

#endif
