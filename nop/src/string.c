#include <i586/instr.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

void *memset(void *ptr, int val, size_t n) {
  // 0-byte blocks are not accepted here, sorry :(
  if (!n) return ptr;

  // Converts, for example, 0xA5 to 0xA5A5A5A5,
  // to write easier in 32-bit blocks
  uint32_t val_32 = ((val & 0xFF) << 24) | ((val & 0xFF) << 16) | ((val & 0xFF) << 8) | ((val & 0xFF) << 0);

  // x86-specific magic to fill out that memory block
  i586_rep_stosd(val_32, n >> 2, ptr);
  i586_rep_stosb(val, n & 3, ptr + (n >> 2));

  return ptr;
}

void *memcpy(void *dest, const void *src, size_t n) {
  // x86-specific magic to copy the memory block
  i586_rep_movsd(n >> 2, dest, src);
  i586_rep_movsb(n & 3, dest, src);

  return dest;
}

char *strcpy(char *dest, const char *src) {
  // Copy until we find a null byte
  while (*src) *dest++ = *src++;

  // Return the destination
  return dest;
}


char *strncpy(char *dest, const char *src, size_t n) {
  // Copy until we find a null byte or n becomes zero
  while (n && *src) *dest++ = *src++, n--;

  // Fill the rest of n(if n > length) with null bytes
  memset(dest, 0, n);

  // Return the destination
  return dest;
}

int strcmp(const char *str_1, const char *str_2) {
  // Find either the end of the strings or the last
  // equal char
  while (*str_1 && *str_1 == *str_2 && *str_2) str_1++, str_2++;

  // Return the signed difference between the last
  // chars
  return ((int)(*str_1) - (int)(*str_2));
}

const char *strchr(const char *str, int c) {
  // Loop until we find the char or we reach
  // the end of the string
  while (*str) {
    // Return if equal
    if (*str == (char)(c)) return str;

    // Increment the pointer and repeat again
    str++;
  }

  // We didn't find it :(
  return NULL;
}
