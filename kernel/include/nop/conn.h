#ifndef __NOP_CONN_H__
#define __NOP_CONN_H__

#include <nop/type.h>

#define CONN_MAX_HAND 64

typedef enum conn_seek_t conn_seek_t;
typedef enum conn_attr_t conn_attr_t;

typedef struct conn_node_t conn_node_t;
typedef struct conn_hand_t conn_hand_t;

typedef struct conn_t conn_t;

enum conn_seek_t {
  conn_seek_set,
  conn_seek_rel,
  conn_seek_end
};

enum conn_attr_t {
  conn_attr_write = (1 << 0), // 1 if writable
  conn_attr_read =  (1 << 1), // 1 if readable
  conn_attr_size =  (1 << 2), // 0 if fixed size, 1 if resizable
  conn_attr_seek =  (1 << 3), // 0 if queue, 1 otherwise
  conn_attr_exec =  (1 << 4), // 1 if executable
  conn_attr_list =  (1 << 5)  // 0 if file/queue/block, 1 if directory
};

struct conn_node_t {
  char name[55];
  size_t size;

  conn_attr_t attr;
};

struct conn_hand_t {
  int used;
  char name[5];

  void *data;
  
  ssize_t (*init)(conn_hand_t *hand, void *data);
  void    (*free)(void);

  void (*connect)(conn_t *conn, const char *path);
  void (*release)(conn_t *conn);

  ssize_t (*write)(conn_t *conn, void *buffer, size_t count);
  ssize_t (*read)(conn_t *conn, void *buffer, size_t count);
  ssize_t (*size)(conn_t *conn, size_t size);
};

struct conn_t {
  conn_node_t node;

  size_t offset;
  void *data;

  conn_hand_t *handler;
};

extern conn_hand_t *conn_hand;
extern size_t conn_count;

ssize_t conn_init(conn_hand_t hand, void *data);
void    conn_free(const char *name);

conn_t *conn_connect(const char *path);
void    conn_release(conn_t *conn);

ssize_t conn_write(conn_t *conn, void *buffer, size_t count);
ssize_t conn_read(conn_t *conn, void *buffer, size_t count);
ssize_t conn_size(conn_t *conn, size_t size);

ssize_t conn_seek(conn_t *conn, ssize_t offset, conn_seek_t type);
ssize_t conn_tell(conn_t *conn);

#endif
