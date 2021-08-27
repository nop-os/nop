#ifndef __DEVS_FBR_TTY_H__
#define __DEVS_FBR_TTY_H__

#include <boot/tinyboot.h>
#include <arch/i586.h>
#include <nop/type.h>
#include <nop/conn.h>
#include <nop/idt.h>

typedef struct fbr_tty_t fbr_tty_t;

struct fbr_tty_t {
  conn_t *conn;
  tb_vid_t table;

  int pos_x, pos_y;

  int ansi_set, ansi_cnt;
  char ansi_buf[12];

  size_t fore_color, back_color;

  size_t idt_idx;
  size_t idt_cnt;
};

extern conn_hand_t fbr_tty_hand;
extern uint8_t fbr_tty_font[];
extern size_t fbr_tty_height, fbr_tty_scale_x, fbr_tty_scale_y, fbr_tty_speed;

void fbr_tty_scroll(fbr_tty_t *fbr_tty);
void fbr_tty_putchr(fbr_tty_t *fbr_tty, char chr);

void fbr_tty_blink(i586_regs_t *regs, idt_hand_t *hand);

ssize_t fbr_tty_init(conn_hand_t *hand, void *data);
void    fbr_tty_free(conn_hand_t *hand);

void fbr_tty_connect(conn_t *conn, const char *path);

ssize_t fbr_tty_write(conn_t *conn, void *buffer, size_t count);
ssize_t fbr_tty_read(conn_t *conn, void *buffer, size_t count);

void fbr_tty_init_all(void);

#endif
