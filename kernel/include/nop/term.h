#ifndef __NOP_DEBUG_H__
#define __NOP_DEBUG_H__

#include <boot/tinyboot.h>
#include <nop/type.h>

#define TERM_BLACK        "\x1B[30m"
#define TERM_RED          "\x1B[31m"
#define TERM_GREEN        "\x1B[32m"
#define TERM_BROWN        "\x1B[33m"
#define TERM_BLUE         "\x1B[34m"
#define TERM_PURPLE       "\x1B[35m"
#define TERM_CYAN         "\x1B[36m"
#define TERM_LIGHT_GRAY   "\x1B[37m"
#define TERM_GRAY         "\x1B[90m"
#define TERM_LIGHT_RED    "\x1B[91m"
#define TERM_LIGHT_GREEN  "\x1B[92m"
#define TERM_YELLOW       "\x1B[93m"
#define TERM_LIGHT_BLUE   "\x1B[94m"
#define TERM_LIGHT_PURPLE "\x1B[95m"
#define TERM_LIGHT_CYAN   "\x1B[96m"
#define TERM_WHITE        "\x1B[97m"

#ifdef NOP_DEBUG
#define term_infof(...) term_printf(TERM_WHITE "[" TERM_LIGHT_BLUE  "INFO" TERM_WHITE " " __FILE__ "] " TERM_LIGHT_GRAY __VA_ARGS__)
#define term_donef(...) term_printf(TERM_WHITE "[" TERM_LIGHT_GREEN "DONE" TERM_WHITE " " __FILE__ "] " TERM_LIGHT_GRAY __VA_ARGS__)
#define term_warnf(...) term_printf(TERM_WHITE "[" TERM_YELLOW      "WARN" TERM_WHITE " " __FILE__ "] " TERM_LIGHT_GRAY __VA_ARGS__)
#define term_failf(...) term_printf(TERM_WHITE "[" TERM_LIGHT_RED   "FAIL" TERM_WHITE " " __FILE__ "] " TERM_LIGHT_GRAY __VA_ARGS__)
#else
#define term_infof(...)
#define term_donef(...)
#define term_warnf(...)
#define term_failf(...)
#endif

extern tb_vid_t *term_table;
extern int16_t term_x, term_y;

extern uint32_t term_fore, term_back;

extern char term_ansi[];
extern int term_length;

extern const uint8_t term_colors[];
extern const uint8_t term_font[];

extern const uint8_t term_map_16[];

uint32_t term_color(uint32_t red, uint32_t green, uint32_t blue);

void term_init(tb_vid_t *table);
void term_panic(void);

void term_scroll(void);
void term_cursor(void);

void term_putchr(char chr);
void term_putstr(const char *str);
void term_putnum(int num, int base, int upper);

void term_write(const char *str, size_t size);

void term_putstr_opt(const char *str, int pad_aln, int pad_len, char pad_chr);
void term_putnum_opt(int num, int base, int upper, int pad_aln, int pad_len, char pad_chr);

void term_printf(const char *format, ...);

#endif
