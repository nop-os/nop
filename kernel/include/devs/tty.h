#ifndef __DEVS_TTY_H__
#define __DEVS_TTY_H__

#include <nop/type.h>
#include <nop/conn.h>

#define TTY_PORT_0 ((void *)(0x03F8))
#define TTY_PORT_1 ((void *)(0x02F8))
#define TTY_PORT_2 ((void *)(0x03E8))
#define TTY_PORT_3 ((void *)(0x02E8))

extern conn_hand_t tty_hand;

void tty_init_all(void);

ssize_t tty_init(conn_hand_t *hand, void *data);
void    tty_connect(conn_t *conn, const char *path);

ssize_t tty_write(conn_t *conn, void *buffer, size_t count);
ssize_t tty_read(conn_t *conn, void *buffer, size_t count);

#endif