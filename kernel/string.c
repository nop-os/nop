#include <arch/i586.h>
#include <nop/type.h>
#include <string.h>

void *memcpy(void *dest, const void *src, size_t size) {
  if (!size) return dest;
  
  i586_rep_movsd((uint32_t)(size >> 2), dest, src);
  i586_rep_movsb((uint32_t)(size & 3), dest + (size & 0xFFFFFFFC), src + (size & 0xFFFFFFFC));
  
  return dest;
}

void *memset(void *ptr, int val, size_t size) {
  if (!size) return ptr;
  
  i586_rep_stosb((uint8_t)(val), (uint32_t)(size), ptr);
  return ptr;
}

int memcmp(const char *str_1, const char *str_2, size_t size) {
  while (size) {
    if (*str_1 != *str_2) {
      return (int)(*str_1) - (int)(*str_2);
    }

    str_1++, str_2++, size--;
  }

  return 0;
}

void *memmove(void *dest, const void *src, size_t size) {
  // TODO: check bounds and all that
  return memcpy(dest, src, size);
}

size_t strlen(const char *str) {
  size_t size = 0;
  
  while (*str) {
    str++, size++;
  }
  
  return size;
}

char *strcat(char *dest, const char *src) {
  return strcpy(dest + strlen(dest), src);
}

char *strncat(char *dest, const char *src, size_t size) {
  return strncpy(dest + strlen(dest), src, size);
}

char *strcpy(char *dest, const char *src) {
  return memcpy(dest, src, strlen(src) + 1);
}

char *strncpy(char *dest, const char *src, size_t size) {
  size_t str_size = strlen(src) + 1;
  if (str_size > size) str_size = size;

  return memcpy(dest, src, str_size);
}

int strcmp(const char *str_1, const char *str_2) {
  while (*str_1) {
    if (*str_1 != *str_2) break;
    str_1++, str_2++;
  }

  return (int)(*str_1) - (int)(*str_2);
}

int strncmp(const char *str_1, const char *str_2, size_t size) {
  if (!size) return 0;
  
  while (*str_1 && size > 1) {
    if (*str_1 != *str_2) break;
    
    str_1++, str_2++;
    size--;
  }

  return (int)(*str_1) - (int)(*str_2);
}

char *strchr(const char *str, int val) {
  while (*str) {
    if (*str == (char)(val)) {
      return (char *)(str);
    }
    
    str++;
  }
  
  return NULL;
}

char *strrchr(const char *str, int val) {
  const char *last = NULL;
  
  while (*str) {
    if (*str == (char)(val)) {
      last = str;
    }
    
    str++;
  }
  
  return (char *)(last);
}
