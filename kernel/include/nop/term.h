#ifndef __NOP_DEBUG_H__
#define __NOP_DEBUG_H__

#include <boot/tinyboot.h>
#include <nop/type.h>

#define DBG_BLACK        "\x1B[30m"
#define DBG_RED          "\x1B[31m"
#define DBG_GREEN        "\x1B[32m"
#define DBG_BROWN        "\x1B[33m"
#define DBG_BLUE         "\x1B[34m"
#define DBG_PURPLE       "\x1B[35m"
#define DBG_CYAN         "\x1B[36m"
#define DBG_LIGHT_GRAY   "\x1B[37m"
#define DBG_GRAY         "\x1B[90m"
#define DBG_LIGHT_RED    "\x1B[91m"
#define DBG_LIGHT_GREEN  "\x1B[92m"
#define DBG_YELLOW       "\x1B[93m"
#define DBG_LIGHT_BLUE   "\x1B[94m"
#define DBG_LIGHT_PURPLE "\x1B[95m"
#define DBG_LIGHT_CYAN   "\x1B[96m"
#define DBG_WHITE        "\x1B[97m"

#define term_infof(...) term_printf(DBG_WHITE "[" DBG_LIGHT_BLUE  "INFO" DBG_WHITE " " __FILE__ "] " DBG_LIGHT_GRAY __VA_ARGS__)
#define term_donef(...) term_printf(DBG_WHITE "[" DBG_LIGHT_GREEN "DONE" DBG_WHITE " " __FILE__ "] " DBG_LIGHT_GRAY __VA_ARGS__)
#define term_warnf(...) term_printf(DBG_WHITE "[" DBG_YELLOW      "WARN" DBG_WHITE " " __FILE__ "] " DBG_LIGHT_GRAY __VA_ARGS__)
#define term_failf(...) term_printf(DBG_WHITE "[" DBG_LIGHT_RED   "FAIL" DBG_WHITE " " __FILE__ "] " DBG_LIGHT_GRAY __VA_ARGS__)

extern tb_vid_t *term_table;
extern uint16_t term_x, term_y;

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

void term_putchr(char chr);
void term_putstr(const char *str);
void term_putnum(int num, int base, int upper);

void term_putstr_opt(const char *str, int pad_aln, int pad_len, char pad_chr);
void term_putnum_opt(int num, int base, int upper, int pad_aln, int pad_len, char pad_chr);

void term_printf(const char *format, ...);

#endif
