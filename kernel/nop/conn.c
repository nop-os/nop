#include <nop/conn.h>
#include <nop/type.h>
#include <nop/mem.h>
#include <string.h>

conn_hand_t *conn_hand = NULL;
size_t conn_count = 0;

void conn_init(const char *name, conn_hand_t handler) {
  if (!conn_hand) {
    if (!(conn_hand = mem_alloc(sizeof(conn_hand_t) * CONN_MAX_HAND))) {
      return;
    }

    memset(conn_hand, 0, sizeof(conn_hand_t) * CONN_MAX_HAND);
  }

  if (conn_count < CONN_MAX_HAND) {
    for (size_t i = 0; i < CONN_MAX_HAND; i++) {
      if (!conn_hand[i].used) {
        memcpy(conn_hand + i, &handler, sizeof(conn_hand_t));
        if (name) strcpy(conn_hand[i].name, name);
        conn_hand[i].used = 1;
        conn_hand[i].init();

        conn_count++;
        return;
      }
    }
  }
}

void conn_free(const char *name) {
  if (conn_count) {
    for (size_t i = 0; i < CONN_MAX_HAND; i++) {
      if (!strcmp(conn_hand[i].name, name)) {
        conn_hand[i].used = 0;
        conn_hand[i].free();

        conn_count--;
        return;
      }
    }
  }
}

conn_t *conn_connect(const char *path) {
  if (conn_count) {
    for (size_t i = 0; i < CONN_MAX_HAND; i++) {
      if (!memcmp(conn_hand[i].name, path, 4)) {
        conn_t *conn = mem_alloc(sizeof(conn_t));
        conn_hand[i].connect(conn, path + 5);

        return conn;
      }
    }
  }

  return NULL;
}

void conn_release(conn_t *conn) {
  if (!conn) return;

  conn->handler->release(conn);
  mem_free(conn);
}

ssize_t conn_write(conn_t *conn, void *buffer, size_t count) {
  if (!conn) return -1;

  if (!(conn->node.attr & conn_attr_write)) return -1;
  return conn->handler->write(conn, buffer, count);
}

ssize_t conn_read(conn_t *conn, void *buffer, size_t count) {
  if (!conn) return -1;

  if (!(conn->node.attr & conn_attr_read)) return -1;
  return conn->handler->read(conn, buffer, count);
}

ssize_t conn_size(conn_t *conn, size_t size) {
  if (!conn) return -1;

  if (!(conn->node.attr & conn_attr_size)) return -1;
  return conn->handler->size(conn, size);
}

ssize_t conn_seek(conn_t *conn, ssize_t offset, conn_seek_t type) {
  if (!conn) return -1;

  if (!(conn->node.attr & conn_attr_seek)) return -1;

  switch (type) {
    case conn_seek_set:
      conn->offset = (size_t)(offset);
      break;
    case conn_seek_rel:
      conn->offset += offset;
      break;
    case conn_seek_end:
      conn->offset = conn->node.size - (size_t)(offset);
      break;
  }

  return (ssize_t)(conn->offset);
}

ssize_t conn_tell(conn_t *conn) {
  if (!conn) return -1;

  if (!(conn->node.attr & conn_attr_seek)) return -1;
  return (ssize_t)(conn->offset);
}