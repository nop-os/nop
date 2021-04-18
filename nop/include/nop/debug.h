#ifndef __NOP_DEBUG_H__
#define __NOP_DEBUG_H__

#include <stdint.h>

extern uint32_t dbg_cursor;
extern uint16_t *dbg_buffer;
extern uint8_t dbg_color;

// Clears the screen!
void dbg_clrscr(void);

// Print char, detect newlines and stuff
void dbg_putchar(char c);

// Print string without automatic newline
void dbg_puts(const char *str);

// Print number in specified base and with
// specified fixed length, padded with chars
// if smaller than length, or use no padding
// if length is 0
void dbg_putn(uint32_t num, int base, char fill, int length);

// Just an implementation of printf()
void dbg_printf(const char *format, ...);

#endif
