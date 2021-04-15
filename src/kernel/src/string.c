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
