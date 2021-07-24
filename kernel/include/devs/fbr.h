#ifndef __DEVS_FBR_H__
#define __DEVS_FBR_H__

#include <boot/tinyboot.h>
#include <nop/type.h>
#include <nop/conn.h>

extern conn_hand_t fbr_hand;

ssize_t fbr_init(conn_hand_t *hand, void *data);
void    fbr_connect(conn_t *conn, const char *path);

ssize_t fbr_write(conn_t *conn, void *buffer, size_t count);
ssize_t fbr_read(conn_t *conn, void *buffer, size_t count);

void fbr_init_all(tb_vid_t *table);

#endif