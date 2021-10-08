#include <arch/i586.h>
#include <nop/type.h>
#include <nop/dbg.h>
#include <stdarg.h>
#include <string.h>

uint16_t dbg_port = 0;

void dbg_init(uint16_t port) {
  i586_outb(0x00, port + 1);
  i586_outb(0x80, port + 3);
  i586_outb(0x03, port + 0);
  i586_outb(0x00, port + 1);
  i586_outb(0x03, port + 3);
  i586_outb(0xC7, port + 2);
  i586_outb(0x0B, port + 4);
  i586_outb(0x1E, port + 4);
  i586_outb(0xAE, port + 0);

  if (i586_inb(port + 0) != 0xAE) {
    dbg_panic();
  }
  
  dbg_port = port;
  i586_outb(0x0F, port + 4);
}

void dbg_panic(void) {
  // for (;;);
}

void dbg_putchr(char chr) {
  if (chr == '\n') {
    dbg_putchr('\r');
  }
  
  while (!(i586_inb(dbg_port + 5) & 0x20));
  i586_outb(chr, dbg_port + 0);
}

void dbg_putstr(const char *str) {
  dbg_putstr_opt(str, 0, 0, ' ');
}

void dbg_putnum(int num, int base, int upper) {
  dbg_putnum_opt(num, base, upper, 0, 0, ' ');
}

void dbg_putstr_opt(const char *str, int pad_aln, int pad_len, char pad_chr) {
  int length = strlen(str) + 1;

  if (pad_aln) {
    while (pad_len >= length) {
      dbg_putchr(pad_chr);
      pad_len--;
    }
  }

  while (*str) {
    dbg_putchr(*str);
    str++;
  }

  if (!pad_aln) {
    while (pad_len >= length) {
      dbg_putchr(pad_chr);
      pad_len--;
    }
  }
}

void dbg_putnum_opt(int num, int base, int upper, int pad_aln, int pad_len, char pad_chr) {
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

  dbg_putstr_opt(buffer + offset, pad_aln, pad_len, pad_chr);
}

void dbg_printf(const char *format, ...) {
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
          dbg_putchr('%');
          break;
        case 'c':
          dbg_putchr((char)(va_arg(args, int)));
          break;
        case 'i': case 'd': case 'u':
          dbg_putnum_opt(va_arg(args, int), 10, 0, pad_aln, pad_len, pad_chr);
          break;
        case 'x':
          dbg_putnum_opt(va_arg(args, int), 16, 0, pad_aln, pad_len, pad_chr);
          break;
        case 'X': case 'p':
          dbg_putnum_opt(va_arg(args, int), 16, 1, pad_aln, pad_len, pad_chr);
          break;
        case 'o':
          dbg_putnum_opt(va_arg(args, int), 8, 0, pad_aln, pad_len, pad_chr);
          break;
        case 'b':
          dbg_putnum_opt(va_arg(args, int), 2, 0, pad_aln, pad_len, pad_chr);
          break;
        case 's':
          dbg_putstr_opt(va_arg(args, const char *), pad_aln, pad_len, pad_chr);
          break;
      }
    } else {
      dbg_putchr(*format);
    }

    format++;
  }
}
