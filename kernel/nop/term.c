#include <boot/tinyboot.h>
#include <arch/i586.h>
#include <nop/term.h>
#include <nop/type.h>
#include <stdarg.h>
#include <string.h>

tb_vid_t *term_table = NULL;
int16_t term_x, term_y, term_width, term_height;

char term_ansi[16] = {0};
int term_length = 0;

uint32_t term_color(uint32_t red, uint32_t green, uint32_t blue) {
  switch (term_table->bpp) {
    case 32:
      return (blue & 0xFF) | ((green & 0xFF) << 8) | ((red & 0xFF) << 16) | 0xFF000000;
  }
  
  return 0;
}

void term_init(tb_vid_t *table) {
  term_table = table;
  term_x = term_y = 0;
  
  if (!term_table->bpp) {
    term_fore = 15;
    term_back = 0;
    
    // TODO: get buffer address using width, before actually overwriting width
    // (altho it's almost always 0x000B8000)
    
    term_table->width = 80 * 8;   // ignore that 8
    term_table->height = 25 * 16; // and that 16
    
    term_table->pitch = term_table->width / 4;
    
    term_table->buffer = (void *)(0x000B8000);
  }
  
  term_width = term_table->width / 8;
  term_height = term_table->height / 16;
  
  for (int i = 0; i < term_table->height / 16; i++) {
    term_scroll();
  }
}

void term_panic(void) {
  i586_cli();
  for (;;);
}

void term_scroll(void) {
  if (term_table->bpp) {
    memcpy(term_table->buffer, term_table->buffer + 16 * term_table->pitch, term_table->pitch * (term_table->height - 16));
    
    for (int i = 1; i <= 16; i++) {
      switch (term_table->bpp) {
        case 32:
          i586_rep_stosd(term_back, term_table->width, term_table->buffer + (term_table->height - i) * term_table->pitch);
          break;
      }
    }
  } else {
    memcpy(term_table->buffer, term_table->buffer + term_table->pitch, term_table->pitch * ((term_table->height - 16) >> 4));
    i586_rep_stosw(term_back, term_table->width >> 3, term_table->buffer + ((term_table->height - 16) >> 4) * term_table->pitch);
  }
}

void term_clear_line(void) {
  if (term_table->bpp) {
    for (int i = 0; i < 16; i++) {
      switch (term_table->bpp) {
        case 32:
          i586_rep_stosd(term_back, term_table->width, term_table->buffer + (term_y + i) * term_table->pitch);
          break;
      }
    }
  } else {
    i586_rep_stosw(term_back, term_table->width >> 3, term_table->buffer + (term_y >> 4) * term_table->pitch);
  }
}

void term_cursor(void) {
  int index = (term_x >> 3) + (term_y >> 4) * (term_table->width >> 3);
  
  i586_outb(0x0F, 0x03D4);
  i586_outb(index >> 0, 0x03D5);
  i586_outb(0x0E, 0x03D4);
  i586_outb(index >> 8, 0x03D5);
}

void term_putchr(char chr) {
  if (chr == '\x1B' || term_length > 0) {
    term_ansi[term_length++] = chr;
    
    if ((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z')) {
      int num = 0;
      
      switch (chr) {
        case 'm':
          for (int i = 2; i < term_length - 1; i++) {
            num *= 10;
            num += term_ansi[i] - '0';
          }
          
          if (num >= 30 && num <= 37) {
            if (term_table->bpp) {
              term_fore = term_color(term_colors[3 * (num - 30) + 0], term_colors[3 * (num - 30) + 1], term_colors[3 * (num - 30) + 2]);
            } else {
              term_fore = num - 30;
            }
          } else if (num >= 90 && num <= 97) {
            if (term_table->bpp) {
              term_fore = term_color(term_colors[3 * (num - 82) + 0], term_colors[3 * (num - 82) + 1], term_colors[3 * (num - 82) + 2]);
            } else {
              term_fore = num - 82;
            }
          }
          
          break;
        
        case 'J':
          for (int i = 2; i < term_length - 1; i++) {
            num *= 10;
            num += term_ansi[i] - '0';
          }
          
          if (num == 2) {
            for (int i = 0; i < term_table->height / 16; i++) {
              term_scroll();
            }
            
            term_x = term_y = 0;
            term_cursor();
          }
          
          break;
        
        case 'K':
          for (int i = 2; i < term_length - 1; i++) {
            num *= 10;
            num += term_ansi[i] - '0';
          }
          
          if (num == 2) {
            term_clear_line();
          }
          
          break;
      }
      
      term_length = 0;
    }
    
    return;
  }
  
  if (chr == '\n') {
    term_x = 0;
    term_y += 16;
    
    if (term_y >= term_table->height) {
      term_scroll();
      term_y -= 16;
    }
  } else if (chr == '\r') {
    term_x = 0;
  } else if (chr == '\b') {
    term_x -= 8;
    
    if (term_x < 0) {
      term_x += term_table->width;
      term_y -= 16;
      
      if (term_y < 0) {
        term_y = 0;
      }
    }
    
    term_putchr(' ');
    term_x -= 8;
    
    if (term_x < 0) {
      term_x += term_table->width;
      term_y -= 16;
      
      if (term_y < 0) {
        term_y = 0;
      }
    }
  }
  
  if (chr < 0x20 || chr >= 0x7F) {
    if (!term_table->bpp && term_x < term_table->width) {
      uint16_t *buffer = term_table->buffer;
      int index = (term_x >> 3) + (term_y >> 4) * (term_table->width >> 3);
      
      buffer[index] = (buffer[index] & 0xFF) | (term_map_16[term_fore] << 8) | (term_map_16[term_back] << 12);
    }
    
    term_cursor();
    return;
  }
  
  if (term_table->bpp) {
    uint8_t prev = 0x00;
    
    for (int y = 0; y < 16; y++) {
      size_t index = ((16 * (chr >> 4) + y) << 4) + (chr & 0x0F);
      uint8_t byte = 0x00;
      
      if (index < 2048) {
        byte = 0xFF ^ term_font[index];
      }
      
      size_t offset = (term_x + (term_y * term_table->width)) * (term_table->bpp >> 3);
      
      for (int x = 0; x < 8; x++) {
        int full = ((byte >> (7 - x)) & 1);
        
        uint32_t color = full ? term_fore : term_back;
        memcpy(term_table->buffer + (term_x + x) * (term_table->bpp >> 3) + (term_y + y) * term_table->pitch, &color, (term_table->bpp >> 3));
      }
      
      prev = byte;
    }
  } else {
    uint16_t *buffer = term_table->buffer;
    int index = (term_x >> 3) + (term_y >> 4) * (term_table->width >> 3);
    
    buffer[index] = (uint16_t)(chr) | (term_map_16[term_fore] << 8) | (term_map_16[term_back] << 12);
  }
  
  term_x += 8;
  
  if (term_x >= term_table->width) {
    term_x = 0;
    term_y += 16;
    
    if (term_y >= term_table->height) {
      term_scroll();
      term_y -= 16;
    }
  }
  
  if (!term_table->bpp && term_x < term_table->width) {
    uint16_t *buffer = term_table->buffer;
    int index = (term_x >> 3) + (term_y >> 4) * (term_table->width >> 3);
    
    buffer[index] = (buffer[index] & 0xFF) | (term_map_16[term_fore] << 8) | (term_map_16[term_back] << 12);
  }
  
  term_cursor();
}

void term_putstr(const char *str) {
  term_putstr_opt(str, 0, 0, ' ');
}

void term_putnum(int num, int base, int upper) {
  term_putnum_opt(num, base, upper, 0, 0, ' ');
}

void term_write(const char *str, size_t size) {
  while (size--) term_putchr(*(str++));
}

void term_putstr_opt(const char *str, int pad_aln, int pad_len, char pad_chr) {
  int length = strlen(str) + 1;

  if (pad_aln) {
    while (pad_len >= length) {
      term_putchr(pad_chr);
      pad_len--;
    }
  }

  while (*str) {
    term_putchr(*str);
    str++;
  }

  if (!pad_aln) {
    while (pad_len >= length) {
      term_putchr(pad_chr);
      pad_len--;
    }
  }
}

void term_putnum_opt(int num, int base, int upper, int pad_aln, int pad_len, char pad_chr) {
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

  term_putstr_opt(buffer + offset, pad_aln, pad_len, pad_chr);
}

void term_printf(const char *format, ...) {
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
          term_putchr('%');
          break;
        case 'c':
          term_putchr((char)(va_arg(args, int)));
          break;
        case 'i': case 'd': case 'u':
          term_putnum_opt(va_arg(args, int), 10, 0, pad_aln, pad_len, pad_chr);
          break;
        case 'x':
          term_putnum_opt(va_arg(args, int), 16, 0, pad_aln, pad_len, pad_chr);
          break;
        case 'p':
          pad_chr = '0';
          pad_len = 8;
        case 'X':
          term_putnum_opt(va_arg(args, int), 16, 1, pad_aln, pad_len, pad_chr);
          break;
        case 'o':
          term_putnum_opt(va_arg(args, int), 8, 0, pad_aln, pad_len, pad_chr);
          break;
        case 'b':
          term_putnum_opt(va_arg(args, int), 2, 0, pad_aln, pad_len, pad_chr);
          break;
        case 's':
          term_putstr_opt(va_arg(args, const char *), pad_aln, pad_len, pad_chr);
          break;
      }
    } else {
      term_putchr(*format);
    }

    format++;
  }
}
