#include <arch/i586.h>
#include <devs/tty.h>
#include <nop/type.h>
#include <nop/conn.h>
#include <nop/dbg.h>

uint16_t tty_ports[] = {
  0x03F8, 0x02F8, 0x03E8, 0x02E8
};

conn_hand_t tty_hand = (conn_hand_t){
  0, "tty", NULL,
  tty_init, NULL,
  tty_connect, NULL,
  tty_write, tty_read, NULL
};

ssize_t tty_init(conn_hand_t *hand, void *data) {
  int id = (int)(data);
  uint16_t port = tty_ports[id];

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
    dbg_failf("tty: port %d (%04X) does not respond\n", id, port);
    return 0;
  }

  hand->data = (void *)((size_t)(port));
  dbg_donef("tty: port %d (%04X) initialized\n", id, port);

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

  for (size_t i = 0; i < count; i++) {
    if (text[i] == '\n') {
      while (!(i586_inb(port + 5) & 0x20));
      i586_outb('\r', port + 0);
    }

    while (!(i586_inb(port + 5) & 0x20));
    i586_outb(text[i], port + 0);
  }

  return (ssize_t)(count);
}

ssize_t tty_read(conn_t *conn, void *buffer, size_t count) {
  char *text = buffer;
  uint16_t port = (uint16_t)((size_t)(conn->data));

  for (size_t i = 0; i < count; i++) {
    while (!(i586_inb(port + 5) & 0x01));
    text[i] = i586_inb(port + 0);
  }

  return (ssize_t)(count);
}

void tty_init_all(void) {
  ssize_t count = 0;

  for (int i = 0; i < 4; i++) {
    void *data = (void *)(i);
    count += conn_init(tty_hand, data);
  }

  dbg_infof("tty: %d ports initialized\n", count);
}