#include <boot/tinyboot.h>
#include <devs/fbr_tty.h>
#include <arch/i586.h>
#include <devs/fbr.h>
#include <nop/conn.h>
#include <nop/type.h>
#include <nop/mem.h>
#include <nop/dbg.h>
#include <nop/idt.h>
#include <string.h>

conn_hand_t fbr_tty_hand = (conn_hand_t){
  0, "tty", NULL,
  fbr_tty_init, fbr_tty_free,
  fbr_tty_connect, NULL,
  fbr_tty_write, fbr_tty_read, NULL
};

void fbr_tty_scroll(fbr_tty_t *fbr_tty) {
  size_t width = fbr_tty->table.width * (fbr_tty->table.bpp >> 3);
  size_t height = fbr_tty_height * fbr_tty_scale_y;

  uint8_t buffer[width];

  for (size_t i = height; i < fbr_tty->table.height; i++) {
    conn_seek(fbr_tty->conn, fbr_tty->table.pitch * i + sizeof(tb_vid_t), conn_seek_set);
    conn_read(fbr_tty->conn, buffer, width);

    conn_seek(fbr_tty->conn, fbr_tty->table.pitch * (i - height) + sizeof(tb_vid_t), conn_seek_set);
    conn_write(fbr_tty->conn, buffer, width);
  }

  memset(buffer, 0, width);

  for (size_t i = 1; i <= height; i++) {
    conn_seek(fbr_tty->conn, fbr_tty->table.pitch * (fbr_tty->table.height - i) + sizeof(tb_vid_t), conn_seek_set);
    conn_write(fbr_tty->conn, buffer, width);
  }

  fbr_tty->pos_y--;
}

void fbr_tty_putchr(fbr_tty_t *fbr_tty, char chr) {
  size_t width = 8 * fbr_tty_scale_x;
  size_t height = fbr_tty_height * fbr_tty_scale_y;

  if (chr == '\n') {
    fbr_tty->pos_x = 0;
    fbr_tty->pos_y++;

    while (fbr_tty->pos_y >= fbr_tty->table.height / height) {
      fbr_tty_scroll(fbr_tty);
    }

    return;
  }

  while (fbr_tty->pos_x >= fbr_tty->table.width / width) {
    fbr_tty->pos_x -= fbr_tty->table.width / width;
    fbr_tty->pos_y++;
  }

  while (fbr_tty->pos_y >= fbr_tty->table.height / height) {
    fbr_tty_scroll(fbr_tty);
  }

  for (size_t y = 0; y < height; y++) {
    size_t index = ((fbr_tty_height * (chr >> 4) + (y / fbr_tty_scale_y)) << 4) + (chr & 0x0F);
    uint8_t byte = fbr_tty_font[index];

    size_t pixel_x = fbr_tty->pos_x * width;
    size_t pixel_y = (fbr_tty->pos_y * height) + y;
    size_t byte_step = fbr_tty->table.bpp >> 3;

    size_t offset = (pixel_x * byte_step) + (pixel_y * fbr_tty->table.pitch);
    conn_seek(fbr_tty->conn, sizeof(tb_vid_t) + offset, conn_seek_set);

    for (size_t x = 0; x < 8; x++) {
      size_t color = ((byte >> (7 - x)) & 0x01) ? fbr_tty->fore_color : fbr_tty->back_color;

      for (size_t sx = 0; sx < fbr_tty_scale_x; sx++) {
        conn_write(fbr_tty->conn, &color, byte_step);
      }
    }
  }

  fbr_tty->pos_x++;
}

void fbr_tty_blink(i586_regs_t *regs, idt_hand_t *hand) {
  fbr_tty_t *fbr_tty = hand->data;

  if (!(fbr_tty->idt_cnt % (fbr_tty_speed / 2))) {
    size_t color = fbr_tty->fore_color;
    size_t pos_x = fbr_tty->pos_x, pos_y = fbr_tty->pos_y;

    if (fbr_tty->idt_cnt >= fbr_tty_speed / 2) {
      fbr_tty->fore_color = fbr_tty->back_color;
    }

    fbr_tty_putchr(fbr_tty, '_');

    fbr_tty->fore_color = color;
    fbr_tty->pos_x = pos_x, fbr_tty->pos_y = pos_y;
  }

  fbr_tty->idt_cnt = (fbr_tty->idt_cnt + 1) % fbr_tty_speed;
}

ssize_t fbr_tty_init(conn_hand_t *hand, void *data) {
  hand->data = data;
  
  return 1;
}

void fbr_tty_free(conn_hand_t *hand) {
  mem_free(hand->data);
}

void fbr_tty_connect(conn_t *conn, const char *path) {
  conn->node.name[0] = '\0';
  conn->node.attr = conn_attr_write | conn_attr_read;

  conn->data = conn->handler->data;
  fbr_tty_t *fbr_tty = conn->data;

  idt_hand_t hand = (idt_hand_t){
    fbr_tty_blink,
    fbr_tty,
    IDT_CLK
  };

  fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0xFF, 0xFF, 0xFF);
  fbr_tty->back_color = fbr_color(fbr_tty->table.bpp, 0x00, 0x00, 0x00);
  fbr_tty->idt_idx = idt_add(hand);
  fbr_tty->idt_cnt = 0;

  fbr_tty->pos_x = fbr_tty->pos_y = 0;
  fbr_tty->ansi_set = 0;
}

ssize_t fbr_tty_write(conn_t *conn, void *buffer, size_t count) {
  fbr_tty_t *fbr_tty = conn->data;
  char *text = buffer;

  for (size_t i = 0; i < count; i++) {
    if (text[i] == '\033') {
      fbr_tty->ansi_set = 1;
      fbr_tty->ansi_cnt = 0;
    }

    if (!fbr_tty->ansi_set) {
      fbr_tty_putchr(fbr_tty, text[i]);
    } else {
      fbr_tty->ansi_buf[fbr_tty->ansi_cnt++] = text[i];
    }

    if (text[i] == 'm') {
      if (!strcmp(fbr_tty->ansi_buf, DBG_BLACK)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0x00, 0x00, 0x00);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_RED)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0xAA, 0x00, 0x00);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_GREEN)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0x00, 0xAA, 0x00);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_BROWN)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0xAA, 0x55, 0x00);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_BLUE)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0x00, 0x00, 0xAA);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_PURPLE)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0xAA, 0x00, 0xAA);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_CYAN)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0x00, 0xAA, 0xAA);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_LIGHT_GRAY)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0xAA, 0xAA, 0xAA);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_GRAY)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0x55, 0x55, 0x55);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_LIGHT_RED)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0xFF, 0x55, 0x55);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_LIGHT_GREEN)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0x55, 0xFF, 0x55);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_YELLOW)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0xFF, 0xFF, 0x55);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_LIGHT_BLUE)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0x55, 0x55, 0xFF);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_LIGHT_PURPLE)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0xFF, 0x55, 0xFF);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_LIGHT_CYAN)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0x55, 0xFF, 0xFF);
      } else if (!strcmp(fbr_tty->ansi_buf, DBG_WHITE)) {
        fbr_tty->fore_color = fbr_color(fbr_tty->table.bpp, 0xFF, 0xFF, 0xFF);
      }

      fbr_tty->ansi_set = 0;
    }
  }

  fbr_tty->idt_cnt = 1;

  return (ssize_t)(count);
}

ssize_t fbr_tty_read(conn_t *conn, void *buffer, size_t count) {
  return 0;
}

void fbr_tty_init_all(void) {
  for (size_t i = 0; i < conn_count; i++) {
    if (!memcmp(conn_hand[i].name, "fbr", 3)) {
      conn_t *conn = conn_connect(conn_hand[i].name);
      if (!conn) continue;

      fbr_tty_t *fbr_tty = mem_alloc(sizeof(fbr_tty_t));
      fbr_tty->conn = conn;

      conn_seek(fbr_tty->conn, 0, conn_seek_set);
      conn_read(fbr_tty->conn, &(fbr_tty->table), sizeof(tb_vid_t));
      conn_init(fbr_tty_hand, fbr_tty);
    }
  }
}
