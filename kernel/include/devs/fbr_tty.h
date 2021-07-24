#ifndef __DEVS_FBR_TTY_H__
#define __DEVS_FBR_TTY_H__

#include <nop/type.h>
#include <nop/conn.h>

typedef struct fbr_tty_t fbr_tty_t;

struct fbr_tty_t {
  conn_t *fbr;
  int pos_x, pos_y;
};

extern conn_hand_t fbr_tty_hand;
extern uint8_t fbr_tty_font[];

ssize_t fbr_tty_init(conn_hand_t *hand, void *data);
void    fbr_tty_connect(conn_t *conn, const char *path);
void    fbr_tty_release(conn_t *conn);

ssize_t fbr_tty_write(conn_t *conn, void *buffer, size_t count);
ssize_t fbr_tty_read(conn_t *conn, void *buffer, size_t count);

void fbr_tty_init_all(tb_vid_t *table);

#endif
