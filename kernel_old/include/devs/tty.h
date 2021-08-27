#ifndef __DEVS_TTY_H__
#define __DEVS_TTY_H__

#include <nop/type.h>
#include <nop/conn.h>

extern uint16_t tty_ports[];
extern conn_hand_t tty_hand;

ssize_t tty_init(conn_hand_t *hand, void *data);
void    tty_connect(conn_t *conn, const char *path);

ssize_t tty_write(conn_t *conn, void *buffer, size_t count);
ssize_t tty_read(conn_t *conn, void *buffer, size_t count);

void tty_init_all(void);

#endif