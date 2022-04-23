#include <nop/call.h>
#include <nop/file.h>
#include <nop/term.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int putstr_opt(const char *str, int pad_aln, int pad_len, char pad_chr) {
  int length = strlen(str);
  
  if (pad_aln) {
    while (pad_len > length) {
      if (putchar(pad_chr) < 0) return EOF;
      pad_len--;
    }
  }
  
  if (!term_write(str, length)) return EOF;
  
  if (!pad_aln) {
    while (pad_len > length) {
      if (putchar(pad_chr) < 0) return EOF;
      pad_len--;
    }
  }
  
  return length;
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
  if (term_write((void *)(&chr), 1)) return chr;
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
  while (!term_read((void *)(&chr), 1));
  
  return chr;
}

char *gets_s(char *buffer, size_t size) {
  size_t read;
  while (!(read = term_read(buffer, size - 1)));
  
  buffer[read] = '\0';
  return buffer;
}

FILE *fopen(const char *path, const char *mode) {
  int file = file_open(path);
  if (!file) return NULL;
  
  FILE *ptr = malloc(sizeof(FILE));
  
  ptr->file = file;
  ptr->read_only = 1; // TODO: parse mode
  
  return ptr;
}

int fclose(FILE *file) {
  if (!file) return EOF;
  if (!file_close(file->file, !file->read_only)) return EOF;
  
  free(file);
  return 0;
}

size_t fread(void *ptr, size_t size, size_t count, FILE *file) {
  if (!file) return 0;
  return file_read(file->file, ptr, size * count);
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *file) {
  if (!file) return 0;
  return file_write(file->file, ptr, size * count);
}

int fseek(FILE *file, off_t offset, int type) {
  if (!file) return EOF;
  
  file_seek(file->file, offset, type);
  return 0;
}

off_t ftell(FILE *file) {
  if (!file) return EOF;
  return file_seek(file->file, 0, SEEK_CUR);
}

int feof(FILE *file) {
  off_t curr = ftell(file);
  
  fseek(file, 0, SEEK_END);
  off_t size = ftell(file);
  
  if (curr == size) return 1;
  
  fseek(file, curr, SEEK_SET);
  return 0;
}

void rewind(FILE *file) {
  fseek(file, 0, SEEK_SET);
}
