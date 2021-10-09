#include <nop/send.h>
#include <nop/type.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

FILE stdio_files[STDIO_FILE_MAX];

void stdio_init(int prog_in, int prog_out) {
  for (int i = 0; i < STDIO_FILE_MAX; i++) {
    stdio_files[i].free =  (i >= 3);
    stdio_files[i].prog = !(i >= 3);
  }
  
  stdin->id =  prog_in;
  stdout->id = prog_out;
  stderr->id = prog_out;
  
  nop_send(prog_out, "ECHO", 0, 0, 0);
}

int fputc(int chr, FILE *file) {
  if (file < stdio_files || file >= stdio_files + STDIO_FILE_MAX) {
    return EOF;
  } else if (file->free) {
    return EOF;
  }
  
  if (file->prog) {
    nop_send(file->id, "WRIT", (uint32_t)(&chr), 1, 0);
  } else {
    nop_send(0, "WRIT", file->id, (uint32_t)(&chr), 1);
  }
  
  return chr;
}

int fputs_opt(const char *str, FILE *file, int pad_aln, int pad_len, char pad_chr) {
  int length = strlen(str);

  if (pad_aln) {
    while (pad_len >= length) {
      if (fputc(pad_chr, file) == EOF) {
        return EOF;
      }
      
      pad_len--;
    }
  }

  while (*str) {
    if (fputc(*str, file) == EOF) {
      return EOF;
    }
    
    str++;
  }
  
  if (!pad_aln) {
    while (pad_len >= length) {
      if (fputc(pad_chr, file) == EOF) {
        return EOF;
      }
      
      pad_len--;
    }
  }
  
  return 0;
}

int fputn_opt(int num, int base, int upper, FILE *file, int pad_aln, int pad_len, char pad_chr) {
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

  return fputs_opt(buffer + offset, file, pad_aln, pad_len, pad_chr);
}

int fprintf(FILE *file, const char *format, ...) {
  va_list args;
  va_start(args, format);

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
          if (fputc('%', file) == EOF) {
            return EOF;
          }
          
          break;
          
        case 'c':
          if (fputc((char)(va_arg(args, int)), file) == EOF) {
            return EOF;
          }
          
          break;
          
        case 'i': case 'd': case 'u':
          fputn_opt(va_arg(args, int), 10, 0, file, pad_aln, pad_len, pad_chr);
          break;
          
        case 'x':
          fputn_opt(va_arg(args, int), 16, 0, file, pad_aln, pad_len, pad_chr);
          break;
          
        case 'X': case 'p':
          fputn_opt(va_arg(args, int), 16, 1, file, pad_aln, pad_len, pad_chr);
          break;
          
        case 'o':
          fputn_opt(va_arg(args, int), 8, 0, file, pad_aln, pad_len, pad_chr);
          break;
          
        case 'b':
          fputn_opt(va_arg(args, int), 2, 0, file, pad_aln, pad_len, pad_chr);
          break;
          
        case 's':
          fputs_opt(va_arg(args, const char *), file, pad_aln, pad_len, pad_chr);
          break;
      }
    } else {
      if (fputc(*format, file) == EOF) {
        return EOF;
      }
    }

    format++;
  }
  
  return 1;
}
