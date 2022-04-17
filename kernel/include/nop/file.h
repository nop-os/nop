#ifndef __NOP_FILE_H__
#define __NOP_FILE_H__

#include <nop/type.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define STAT_RO (1 << 0) // read only
#define STAT_HD (1 << 1) // hidden
#define STAT_SY (1 << 2) // system
#define STAT_DR (1 << 3) // directory

typedef struct file_t file_t;

struct file_t {
  int free;
  
  char path[256];
  uint8_t stat;
  
  void *buffer;
  size_t size, offset;
};

extern file_t *file_arr;
extern int file_cnt;

int     file_open(const char *path);
int     file_create(const char *path);
int     file_remove(const char *path);
int     file_close(int id, int save);
uint8_t file_stat(int id, uint8_t stat, int save);

size_t  file_seek(int id, off_t offset, int type);
int     file_size(int id, size_t size);
size_t  file_tell(int id);
int     file_eof(int id);

size_t file_read(int id, void *buffer, size_t size);
size_t file_write(int id, void *buffer, size_t size);

#endif
