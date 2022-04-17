#ifndef __NOP_TIME_H__
#define __NOP_TIME_H__

#include <arch/i586.h>
#include <nop/type.h>

// 1 << 5 = 32 ticks per second
#define TIME_FRACTION 5

// oh really?
#define TIME_YEAR 2022

extern int time_year;
extern int time_month;
extern int time_day;
extern int time_hour;
extern int time_minute;
extern int time_second;
extern int time_step;

extern uint64_t time_raw;
extern int time_hand;

void time_init(void);
void time_tick(i586_regs_t *regs);

uint64_t time_read(void);
void     time_wait(uint64_t count);

#endif
