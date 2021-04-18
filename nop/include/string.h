#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>

// Standard string.h functions

void *memset(void *ptr, int c, size_t n);
void *memcpy(void *dest, const void * src, size_t n);

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);

int strcmp(const char *str_1, const char *str_2);
const char *strchr(const char *str, int c);

#endif
