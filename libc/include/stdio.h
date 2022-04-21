#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdarg.h>
#include <stdint.h>

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define EOF ((int)(-1))

typedef struct FILE FILE;

struct FILE {
  int file, read_only;
};

int putstr_opt(const char *str, int pad_aln, int pad_len, char pad_chr);
int putnum_opt(int num, int base, int upper, int pad_aln, int pad_len, char pad_chr);

int putstr(const char *str);
int putnum(int num);

int putchar(int chr);
int puts(const char *str);

int vprintf(const char *format, va_list args);
int printf(const char *format, ...);

int getchar(void);
char *gets_s(char *buffer, size_t size);

FILE *fopen(const char *path, const char *mode);
int   fclose(FILE *file);

size_t fread(void *ptr, size_t size, size_t count, FILE *file);
size_t fwrite(const void *ptr, size_t size, size_t count, FILE *file);

int   fseek(FILE *file, off_t offset, int type);
off_t ftell(FILE *file);

#endif
