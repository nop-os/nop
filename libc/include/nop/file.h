#ifndef __NOP_FILE_H__
#define __NOP_FILE_H__

#include <stdint.h>

#define FILE_PATH_MAX 256

#define FILE_READ_ONLY (1 << 0)
#define FILE_HIDDEN    (1 << 1)
#define FILE_SYSTEM    (1 << 2)
#define FILE_DIRECTORY (1 << 3)

int file_open(const char *path);
int file_create(const char *path);
int file_delete(const char *path);
int file_close(int id, int save);

size_t file_read(int id, void *buffer, size_t size);
size_t file_write(int id, const void *buffer, size_t size);

int file_entry(int id, char *buffer, size_t size);

size_t file_seek(int id, off_t offset, int type);
int    file_resize(int id, size_t size);

int file_getmode(int id);
int file_setmode(int id, int mode);

#endif
