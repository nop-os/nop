#ifndef __DEVS_FAT_H__
#define __DEVS_FAT_H__

#include <nop/type.h>
#include <nop/conn.h>

extern conn_hand_t fat_hand;

ssize_t fat_init(conn_hand_t *hand, void *data);
void    fat_free(conn_hand_t *hand);

void    fat_connect(conn_t *conn, const char *path);

ssize_t fat_write(conn_t *conn, void *buffer, size_t count);
ssize_t fat_read(conn_t *conn, void *buffer, size_t count);

void fat_init_all(void);

#endif