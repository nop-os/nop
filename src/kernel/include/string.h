#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

void *memset(void *str, int c, size_t n);

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);

#endif
