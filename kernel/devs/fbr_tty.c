#include <devs/fbr_tty.h>
#include <nop/conn.h>
#include <nop/type.h>

conn_hand_t fbr_tty_hand;

ssize_t fbr_tty_init(conn_hand_t *hand, void *data) {
  hand->data = data;
}

void    fbr_tty_connect(conn_t *conn, const char *path);
void    fbr_tty_release(conn_t *conn);

ssize_t fbr_tty_write(conn_t *conn, void *buffer, size_t count);
ssize_t fbr_tty_read(conn_t *conn, void *buffer, size_t count);

void fbr_tty_init_all(tb_vid_t *table);
