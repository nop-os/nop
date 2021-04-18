#include <nop/debug.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>

uint32_t dbg_cursor = 0;
uint16_t *dbg_buffer = (uint16_t *)(0xB8000);
uint8_t dbg_color = 0x07;

void dbg_clrscr(void) {
  // Clears the screen?
  memset(dbg_buffer, 0x00, 4000);
}

void dbg_putchar(char c) {
  // Don't print null bytes
  if (!c) return;

  // If it's a newline, just go to the next line
  if (c == '\n') dbg_cursor += 80 - (dbg_cursor % 80);

  // Otherwise, just put it into the buffer and
  // increment the cursor
  else dbg_buffer[dbg_cursor++] = ((uint16_t)(dbg_color) << 8) | c;
}

void dbg_puts(const char *str) {
  // Pretty obvious: Just loop through each char
  // in the string until you find a null byte!
  while (*str) dbg_putchar(*str++);
}

void dbg_putn(uint32_t num, int base, char fill, int length) {
  // Maybe we could do base 1 by typing as many
  // zeroes as the value of the number, but it's
  // better to just limit it from base 2 to 36
  if (base < 2 || base > 36) return;

  // Array of valid chars
  const char *num_chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  // Temp. buffer, 32 chars + null byte should
  // do the trick, also store an index
  char buffer[33] = {0};
  int index = 0;

  // If the number is 0, then we finished
  // printing it
  while (num) {
    buffer[index++] = num_chars[num % base];
    num /= base;
  }

  // If index(which is also the length) is less
  // than the desired length, print fill bytes
  for (int i = 0; i < length - index; i++) dbg_putchar(fill);

  // If the length is zero and the number is
  // also zero, then just print a poor, lonely
  // zero
  if (!length && !num) dbg_putchar('0');

  // Now print the buffer in reverse order
  while (index) dbg_putchar(buffer[--index]);
}

void dbg_printf(const char *format, ...) {
  // Setup variadic arguments
  va_list params;
  va_start(params, format);

  // Store current char here
  char c = '\0';

  // Temp. format variables
  char tmp_chr = '\0';
  char *tmp_str = NULL;
  int tmp_int = 0;
  unsigned int tmp_uint = 0;

  // Loop through all chars
  while (c = *format++) {
    if (c == '%') {
      // Get next char
      c = *format++;

      // Some values that can be set up with flags
      char fill_char = ' ';
      int fill_length = 0;

      // If it's not a printf format type, then it's
      // the padding or fill char
      if (!strchr("%duxsc", c)) {
        fill_char = c;
        c = *format++;
      }

      // This number has to be the padding or fill
      // length, convert it to an integer
      while (c >= '0' && c <= '9') {
        fill_length = (fill_length * 10) + (c - '0');
        c = *format++;
      }

      // Hey, this is some kind of argument!
      switch (c) {
        case '%':
          // False alarm, the user is just trying to
          // print a percentage sign...
          dbg_putchar('%');
          break;
        case 'd':
          // An integer!
          tmp_int = va_arg(params, int);
          dbg_putn(tmp_int, 10, fill_char, fill_length);
          break;
        case 'u':
          // An unsigned integer!
          tmp_uint = va_arg(params, unsigned int);
          dbg_putn(tmp_uint, 10, fill_char, fill_length);
          break;
        case 'x':
          // An hexadecimal, unsigned integer!
          tmp_uint = va_arg(params, unsigned int);
          dbg_putn(tmp_uint, 16, fill_char, fill_length);
          break;
        case 's':
          // A string!
          tmp_str = va_arg(params, char *);
          dbg_puts(tmp_str);
          break;
        case 'c':
          // A char!
          tmp_chr = (char)(va_arg(params, int));
          dbg_putchar(tmp_chr);
          break;
      }
    } else {
      dbg_putchar(c);
    }
  }
}
