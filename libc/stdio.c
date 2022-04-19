#include <nop/call.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int putstr_opt(const char *str, int pad_aln, int pad_len, char pad_chr) {
  int length = strlen(str);
  
  if (pad_aln) {
    while (pad_len >= length) {
      if (putchar(pad_chr) < 0) return EOF;
      pad_len--;
    }
  }
  
  if (!nop_call("term_write", (uint32_t)(str), length, 0, 0, 0)) return EOF;
  
  if (pad_aln) {
    while (pad_len >= length) {
      if (putchar(pad_chr) < 0) return EOF;
      pad_len--;
    }
  }
  
  return (length > pad_len ? pad_len : length);
}

int putnum_opt(int num, int base, int upper, int pad_aln, int pad_len, char pad_chr) {
  char buffer[100] = {0};
  int offset = 99;

  const char *digits_upper = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const char *digits_lower = "0123456789abcdefghijklmnopqrstuvwxyz";
  const char *digits = (upper ? digits_upper : digits_lower);

  unsigned int value = (unsigned int)(num);

  if (num < 0 && base == 10) {
    value = (unsigned int)(-num);
  }

  if (num == 0) {
    buffer[--offset] = '0';
  } else {
    while (value) {
      buffer[--offset] = digits[value % base];
      value /= base;
    }
  }

  if (num < 0 && base == 10) {
    buffer[--offset] = '-';
  }

  return putstr_opt(buffer + offset, pad_aln, pad_len, pad_chr);
}

int putstr(const char *str) {
  return putstr_opt(str, 0, 0, ' ');
}

int putnum(int num) {
  return putnum_opt(num, 10, 0, 0, 0, ' ');
}

int putchar(int chr) {
  if (nop_call("term_write", (uint32_t)(&chr), 1, 0, 0, 0)) return chr;
  else return EOF;
}

int puts(const char *str) {
  int size = putstr(str);
  if (size < 0) return EOF;
  
  if (putchar('\n') < 0) return EOF;
  return size + 1;
}

int vprintf(const char *format, va_list args) {
  int count = 0;
  
  while (*format) {
    if (*format == '%') {
      format++;

      int pad_aln = 1;
      int pad_len = 0;
      char pad_chr = ' ';

      if (*format == '-') {
        pad_aln = 0;
        format++;
      }

      if (*format == '0') {
        pad_chr = '0';
        format++;
      }

      while (*format >= '0' && *format <= '9') {
        pad_len *= 10;
        pad_len += (*format - '0');
        format++;
      }

      switch (*format) {
        case '%':
          if (putchar('%') < 0) return EOF;
          count++;
          
          break;
          
        case 'c':
          if (putchar(va_arg(args, int)) < 0) return EOF;
          count++;
          
          break;
          
        case 'i': case 'd': case 'u':
          count += putnum_opt(va_arg(args, int), 10, 0, pad_aln, pad_len, pad_chr);
          break;
          
        case 'x':
          count += putnum_opt(va_arg(args, int), 16, 0, pad_aln, pad_len, pad_chr);
          break;
          
        case 'p':
          pad_chr = '0';
          pad_len = 8;
          
        case 'X':
          count += putnum_opt(va_arg(args, int), 16, 1, pad_aln, pad_len, pad_chr);
          break;
          
        case 'o':
          count += putnum_opt(va_arg(args, int), 8, 0, pad_aln, pad_len, pad_chr);
          break;
          
        case 'b':
          count += putnum_opt(va_arg(args, int), 2, 0, pad_aln, pad_len, pad_chr);
          break;
          
        case 's':
          count += putstr_opt(va_arg(args, const char *), pad_aln, pad_len, pad_chr);
          break;
      }
    } else {
      if (putchar(*format) < 0) return EOF;
      count++;
    }

    format++;
  }
  
  return count;
}

int printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  
  return vprintf(format, args);
}

int getchar(void) {
  int chr;
  while (!nop_call("term_read", (uint32_t)(&chr), 1, 0, 0, 0));
  
  return chr;
}

char *gets_s(char *buffer, size_t size) {
  size_t read;
  while (!(read = nop_call("term_read", (uint32_t)(buffer), size - 1, 0, 0, 0)));
  
  buffer[read] = '\0';
  return buffer;
}
