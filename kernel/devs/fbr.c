#include <boot/tinyboot.h>
#include <arch/i586.h>
#include <devs/fbr.h>
#include <nop/type.h>
#include <nop/conn.h>
#include <nop/page.h>
#include <nop/dbg.h>
#include <nop/idt.h>
#include <nop/mem.h>
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

void fbr_refresh(i586_regs_t *regs, idt_hand_t *hand) {
  fbr_t *fbr = hand->data;
  tb_vid_t *table = fbr->table;

  if (!fbr->idt_cnt) {
    memcpy(table->buffer, fbr->buffer, table->height * table->pitch);
  }

  fbr->idt_cnt = (fbr->idt_cnt + 1) % FBR_RATE;
}

ssize_t fbr_init(conn_hand_t *hand, void *data) {
  tb_vid_t *table = data;

  fbr_t *fbr = mem_alloc(sizeof(fbr_t));
  fbr->table = table;
  fbr->buffer = page_alloc((table->height * table->pitch + 0x0FFF) >> 12);

  idt_hand_t fbr_hand = (idt_hand_t){
    fbr_refresh,
    fbr,
    IDT_CLK
  };

  fbr->idt_idx = idt_add(fbr_hand);
  fbr->idt_cnt = 0;

  dbg_donef("fbr: framebuffer initialized\n");
  dbg_infof("- id:     %d\n", fbr->idt_idx);
  dbg_infof("- addr:   0x%08X\n", table->buffer);
  dbg_infof("- buffer: 0x%08X\n", fbr->buffer);
  dbg_infof("- width:  %d\n", table->width);
  dbg_infof("- height: %d\n", table->height);
  dbg_infof("- pitch:  %d\n", table->pitch);
  dbg_infof("- bpp:    %d\n", table->bpp);

  hand->data = fbr;
  return 1;
}

void fbr_connect(conn_t *conn, const char *path) {
  conn->data = conn->handler->data;

  fbr_t *fbr = conn->data;
  tb_vid_t *table = fbr->table;

  conn->node.name[0] = '\0';
  conn->node.attr = conn_attr_write | conn_attr_read | conn_attr_seek;
  conn->node.size = (table->height * table->pitch) + sizeof(tb_vid_t);

  conn->offset = 0;
}

ssize_t fbr_write(conn_t *conn, void *buffer, size_t count) {
  fbr_t *fbr = conn->data;
  tb_vid_t *table = fbr->table;

  if (count > conn->node.size - conn->offset) {
    count = conn->node.size - conn->offset;
  }

  if (conn->offset < sizeof(tb_vid_t)) {
    return -1;
  }

  memcpy(fbr->buffer + (conn->offset - sizeof(tb_vid_t)), buffer, count);
  conn->offset += count;

  return (ssize_t)(count);
}

ssize_t fbr_read(conn_t *conn, void *buffer, size_t count) {
  fbr_t *fbr = conn->data;
  tb_vid_t *table = fbr->table;

  if (count > conn->node.size - conn->offset) {
    count = conn->node.size - conn->offset;
  }

  size_t total = count;

  if (conn->offset < sizeof(tb_vid_t)) {
    size_t table_count = count;

    if (table_count > sizeof(tb_vid_t) - conn->offset) {
      table_count = sizeof(tb_vid_t) - conn->offset;
    }

    memcpy(buffer, (void *)(table) + conn->offset, table_count);
    conn->offset += table_count, buffer += table_count;
    count -= table_count;
  }

  memcpy(buffer, fbr->buffer + (conn->offset - sizeof(tb_vid_t)), count);
  conn->offset += count;

  return (ssize_t)(total);
}

void fbr_init_all(tb_vid_t *table) {
  conn_init(fbr_hand, table);
}