#ifndef __NOP_CONN_H__
#define __NOP_CONN_H__

#include <nop/type.h>

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
  conn_attr_write = 0b00000001, // 1 if writable
  conn_attr_read =  0b00000010, // 1 if readable
  conn_attr_size =  0b00000100, // 0 if fixed size, 1 if resizable
  conn_attr_seek =  0b00001000, // 0 if queue, 1 otherwise
  conn_attr_exec =  0b00010000, // 1 if executable
  conn_attr_list =  0b00100000  // 0 if file/queue/block, 1 if directory
};

struct conn_node_t {
  char name[55];
  size_t size;

  conn_attr_t attr;
};

struct conn_hand_t {
  char name[5];
  
  void (*init)(void);
  void (*free)(void);

  void (*connect)(conn_t *conn);
  void (*release)(conn_t *conn);

  ssize_t (*write)(conn_t *conn, void *buffer, size_t count);
  ssize_t (*read)(conn_t *conn, void *buffer, size_t count);
  ssize_t (*seek)(conn_t *conn, ssize_t offset, conn_seek_t type);
  ssize_t (*size)(conn_t *conn, size_t size);
  ssize_t (*tell)(conn_t *conn);
};

struct conn_t {
  conn_node_t node;

  size_t offset;
  void *data;

  conn_hand_t *handler;
};

void conn_init(const char *name, conn_hand_t handler);
void conn_free(const char *name);

conn_t *conn_connect(const char *path);
void    conn_release(conn_t *conn);

#endif
