#include <arch/i586.h>
#include <nop/term.h>
#include <nop/time.h>
#include <nop/type.h>
#include <nop/idt.h>

int time_year = 2022;
int time_month = 1;
int time_day = 1;
int time_hour = 0;
int time_minute = 0;
int time_second = 0;
int time_step = 0;

uint64_t time_raw = 0;

int time_hand = 0;

static const int time_months[] = {
  31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
  31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static uint8_t time_cmos_read(uint8_t index) {
  i586_outb(index, 0x0070);
  return i586_inb(0x0071);
}

static void time_cmos_send(uint8_t index, uint8_t value) {
  i586_outb(index, 0x0070);
  i586_outb(value, 0x0071);
}

static void time_cmos_wait(void) {
  while (time_cmos_read(0x0A) & 0x80);
}

static uint8_t time_bcd(uint8_t value) {
  return (value & 0x0F) + (value >> 4) * 10;
}

static uint8_t time_cmos(int check) {
  uint8_t flags = time_cmos_read(0x0B);
  if (check) time_cmos_wait();
  
  time_year = time_cmos_read(0x09);
  time_month = time_cmos_read(0x08);
  time_day = time_cmos_read(0x07);
  time_hour = time_cmos_read(0x04);
  time_minute = time_cmos_read(0x02);
  time_second = time_cmos_read(0x00);
  
  if (!(flags & 0x04)) {
    time_year = time_bcd(time_year);
    time_month = time_bcd(time_month);
    time_day = time_bcd(time_day);
    time_hour = time_bcd(time_hour);
    time_minute = time_bcd(time_minute);
    time_second = time_bcd(time_second);
  }
  
  if (!(flags & 0x02) && (time_hour & 0x80)) {
    time_hour = ((time_hour & 0x7F) + 12) % 24;
  }
  
  if (time_year < (TIME_YEAR % 100)) {
    time_year += 100;
  }
  
  time_year += (TIME_YEAR / 100) * 100;
  return flags;
}

void time_init(void) {
  uint8_t flags = time_cmos(1);
  
  time_hand = idt_add(time_tick, IDT_RTC);
  i586_cli();
  
  time_cmos_send(0x8A, 0x20 | (0x10 - TIME_FRACTION));
  time_cmos_send(0x8B, flags | 0x40);
  time_cmos_read(0x0C);
  
  i586_sti();
  term_infof("initialized RTC clock, %02u/%02u/%04u, %02u:%02u:%02u\n", time_day, time_month, time_year, time_hour, time_minute, time_second);
}

void time_tick(i586_regs_t *regs) {
  time_step++;
  time_raw++;
  
  time_second += time_step >> TIME_FRACTION;
  time_step &= (1 << TIME_FRACTION) - 1;
  
  time_minute += time_second / 60;
  time_second %= 60;
  
  time_hour += time_minute / 60;
  time_minute %= 60;
  
  time_day += time_hour / 24;
  time_hour %= 24;
  
  int days_in_month = time_months[time_month + (time_year % 4) * 12];
  
  time_month += (time_day - 1) / days_in_month;
  time_day = ((time_day - 1) % days_in_month) + 1;
  
  time_year += (time_month - 1) / 12;
  time_month = ((time_month - 1) % 12) + 1;
}

static inline uint64_t time_julian(uint8_t days, uint8_t months, uint16_t years) {
  return (1461 * (years + 4800 + (months - 14) / 12)) / 4 + (367 * (months - 2 - 12 * ((months - 14) / 12))) / 12 - (3 * ((years + 4900 + (months - 14) / 12) / 100)) / 4 + days - 32075;
}

uint64_t time_read(void) {
  return ((time_julian(time_day, time_month, time_year) - time_julian(1, 1, 1970)) * 86400 + time_hour * 3600 + time_minute * 60 + time_second) * 1024 + ((time_step * 1024) >> TIME_FRACTION);
}

void time_wait(uint64_t count) {
  uint64_t time = time_read();
  
  while (time_read() - time < count) {
    i586_wait();
  }
}
