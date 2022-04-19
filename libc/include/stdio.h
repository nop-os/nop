#ifndef __STDIO_H__
#define __STDIO_H__

#include <stdarg.h>
#include <stdint.h>

#define EOF ((int)(-1))

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

#endif
