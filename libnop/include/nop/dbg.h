#ifndef __NOP_DBG_H__
#define __NOP_DBG_H__

#include <nop/type.h>

#define DBG_NORMAL    "\033[21m\033[24m"
#define DBG_BOLD      "\033[1m"
#define DBG_UNDERLINE "\033[4m"

#define DBG_BLACK        "\033[30m"
#define DBG_RED          "\033[31m"
#define DBG_GREEN        "\033[32m"
#define DBG_BROWN        "\033[33m"
#define DBG_BLUE         "\033[34m"
#define DBG_PURPLE       "\033[35m"
#define DBG_CYAN         "\033[36m"
#define DBG_LIGHT_GRAY   "\033[37m"
#define DBG_GRAY         "\033[90m"
#define DBG_LIGHT_RED    "\033[91m"
#define DBG_LIGHT_GREEN  "\033[92m"
#define DBG_YELLOW       "\033[93m"
#define DBG_LIGHT_BLUE   "\033[94m"
#define DBG_LIGHT_PURPLE "\033[95m"
#define DBG_LIGHT_CYAN   "\033[96m"
#define DBG_WHITE        "\033[97m"

#define dbg_infof(...) dbg_printf(DBG_BOLD DBG_WHITE "[" DBG_LIGHT_BLUE  "INFO" DBG_WHITE "] " DBG_NORMAL DBG_LIGHT_GRAY __VA_ARGS__)
#define dbg_donef(...) dbg_printf(DBG_BOLD DBG_WHITE "[" DBG_LIGHT_GREEN "DONE" DBG_WHITE "] " DBG_NORMAL DBG_LIGHT_GRAY __VA_ARGS__)
#define dbg_warnf(...) dbg_printf(DBG_BOLD DBG_WHITE "[" DBG_YELLOW      "WARN" DBG_WHITE "] " DBG_NORMAL DBG_LIGHT_GRAY __VA_ARGS__)
#define dbg_failf(...) dbg_printf(DBG_BOLD DBG_WHITE "[" DBG_LIGHT_RED   "FAIL" DBG_WHITE "] " DBG_NORMAL DBG_LIGHT_GRAY __VA_ARGS__)

extern uint16_t dbg_port;

void dbg_init(uint16_t port);
void dbg_panic(void);

void dbg_putchr(char chr);
void dbg_putstr(const char *str);
void dbg_putnum(int num, int base, int upper);

void dbg_putstr_opt(const char *str, int pad_aln, int pad_len, char pad_chr);
void dbg_putnum_opt(int num, int base, int upper, int pad_aln, int pad_len, char pad_chr);

void dbg_printf(const char *format, ...);

#endif
