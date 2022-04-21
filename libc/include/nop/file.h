#ifndef __NOP_FILE_H__
#define __NOP_FILE_H__

#include <stdint.h>

#define FILE_PATH_MAX 256

int file_open(const char *path);
int file_create(const char *path);
int file_delete(const char *path);
int file_close(int id, int save);

size_t file_seek(int id, off_t offset, int type);
int    file_resize(int id, size_t size);

size_t file_read(int id, void *buffer, size_t size);
size_t file_write(int id, const void *buffer, size_t size);

#endif
