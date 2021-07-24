#include <arch/i586.h>
#include <devs/tty.h>
#include <nop/type.h>
#include <nop/conn.h>

conn_hand_t tty_hand = (conn_hand_t){
  0, "tty", NULL,
  tty_init, NULL,
  tty_connect, NULL,
  tty_write, tty_read, NULL
};

void tty_init_all(void) {
  conn_init(tty_hand, TTY_PORT_0);
  conn_init(tty_hand, TTY_PORT_1);
  conn_init(tty_hand, TTY_PORT_2);
  conn_init(tty_hand, TTY_PORT_3);
}

ssize_t tty_init(conn_hand_t *hand, void *data) {
  uint16_t port = (uint16_t)((size_t)(data));

  i586_outb(0x00, port + 1);
  i586_outb(0x80, port + 3);
  i586_outb(0x03, port + 0);
  i586_outb(0x00, port + 1);
  i586_outb(0x03, port + 3);
  i586_outb(0xC7, port + 2);
  i586_outb(0x0B, port + 4);
  i586_outb(0x1E, port + 4);
  i586_outb(0xAE, port + 0);

  if (i586_inb(port + 0) != 0xAE) {
    return 0;
  }

  hand->data = data;

  i586_outb(0x0F, port + 4);
  return 1;
}

void tty_connect(conn_t *conn, const char *path) {
  conn->node.name[0] = '\0';
  conn->node.attr = conn_attr_write | conn_attr_read;
  
  conn->data = conn->handler->data;
}

ssize_t tty_write(conn_t *conn, void *buffer, size_t count) {
  char *text = buffer;
  uint16_t port = (uint16_t)((size_t)(conn->data));

  while (count) {
    if (*text == '\n') {
      while (!(i586_inb(port + 5) & 0x20));
      i586_outb('\r', port + 0);
    }

    while (!(i586_inb(port + 5) & 0x20));
    i586_outb(*text, port + 0);

    text++, count--;
  }

  return (ssize_t)(count);
}

ssize_t tty_read(conn_t *conn, void *buffer, size_t count) {
  char *text = buffer;
  uint16_t port = (uint16_t)((size_t)(conn->data));

  while (count) {
    while (!(i586_inb(port + 5) & 0x01));
    *text = i586_inb(port + 0);

    text++, count--;
  }

  return (ssize_t)(count);
}