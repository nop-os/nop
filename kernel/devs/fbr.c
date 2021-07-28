#include <boot/tinyboot.h>
#include <devs/fbr.h>
#include <nop/type.h>
#include <nop/conn.h>
#include <nop/dbg.h>
#include <string.h>

conn_hand_t fbr_hand = (conn_hand_t){
  0, "fbr", NULL,
  fbr_init, NULL,
  fbr_connect, NULL,
  fbr_write, fbr_read, NULL
};

size_t fbr_color(uint8_t bpp, size_t red, size_t green, size_t blue) {
  switch (bpp) {
    case 0x08:
      return ((red >> 5) << 5) | ((green >> 5) << 2) | (blue >> 6);

    case 0x10:
      return ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);

    case 0x18: case 0x20:
      return (red << 16) | (green << 8) | blue;
  }

  return 0;
}

ssize_t fbr_init(conn_hand_t *hand, void *data) {
  tb_vid_t *table = data;
  hand->data = data;

  dbg_donef("fbr: framebuffer initialized\n");
  dbg_infof("- buffer: 0x%08X\n", table->buffer);
  dbg_infof("- width:  %d\n", table->width);
  dbg_infof("- height: %d\n", table->height);
  dbg_infof("- pitch:  %d\n", table->pitch);
  dbg_infof("- bpp:    %d\n", table->bpp);

  return 1;
}

void fbr_connect(conn_t *conn, const char *path) {
  tb_vid_t *table = conn->handler->data;

  conn->node.name[0] = '\0';
  conn->node.attr = conn_attr_write | conn_attr_read | conn_attr_seek;
  conn->node.size = (table->width * table->pitch) + sizeof(tb_vid_t);

  conn->offset = 0;
  conn->data = conn->handler->data;
}

ssize_t fbr_write(conn_t *conn, void *buffer, size_t count) {
  tb_vid_t *table = conn->data;

  if (count > conn->node.size - conn->offset) {
    count = conn->node.size - conn->offset;
  }

  if (conn->offset < sizeof(tb_vid_t)) {
    return -1;
  }

  memcpy(table->buffer + (conn->offset - sizeof(tb_vid_t)), buffer, count);
  conn->offset += count;

  return (ssize_t)(count);
}

ssize_t fbr_read(conn_t *conn, void *buffer, size_t count) {
  tb_vid_t *table = conn->data;

  if (count > conn->node.size - conn->offset) {
    count = conn->node.size - conn->offset;
  }

  size_t total = count;

  if (conn->offset < sizeof(tb_vid_t)) {
    size_t table_count = count;

    if (table_count > sizeof(tb_vid_t) - conn->offset) {
      table_count = sizeof(tb_vid_t) - conn->offset;
    }

    memcpy(buffer, conn->data + conn->offset, table_count);
    conn->offset += table_count, buffer += table_count;
    count -= table_count;
  }

  memcpy(buffer, table->buffer + (conn->offset - sizeof(tb_vid_t)), count);
  conn->offset += count;

  return (ssize_t)(total);
}

void fbr_init_all(tb_vid_t *table) {
  conn_init(fbr_hand, table);
}