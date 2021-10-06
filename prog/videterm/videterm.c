#include <nop/send.h>
#include <nop/type.h>
#include <nop/dbg.h>
#include <string.h>

#include "font.h"

uint16_t width, height;
uint8_t bpp;

int vide;

int blink_ticks = 0;
int blink_state = 0;

int cursor_x = 0;
int cursor_y = 0;

uint32_t fore_color = 0xFFFFFFFF;
uint32_t back_color = 0xFF000000;

void term_putchar(char c);

__attribute__((__section__(".entry"), __used__))
int nex_start(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  if (type == nop_type("INIT")) {
    vide = nop_find("VIDE");
    if (!vide) return 0;
    
    nop_send(vide, "INFO", (uint32_t)(&width), (uint32_t)(&height), (uint32_t)(&bpp));
    if (bpp != 32) return 0;
    
    nop_send(0, "TIME", 0, 1, 0);
    nop_send(vide, "RECT", 0, width + (height << 16), back_color);
  } else if (type == nop_type("WRIT")) {
    char *str = nop_phys(id, data_1);
    
    while (data_2) {
      term_putchar(*(str++));
      data_2--;
    }
  } else if (type == nop_type("TICK")) {
    if (!blink_ticks) {
      blink_state = 1 - blink_state;
      
      if (blink_state) {
        nop_send(vide, "RECT",
                 (cursor_x * 8 * font_scale_x) + (((cursor_y * font_height + (font_height - 1)) * font_scale_y) << 16),
                 (8 * font_scale_x) + ((1 * font_scale_y) << 16),
                 fore_color
        );
      } else {
        nop_send(vide, "RECT",
                 (cursor_x * 8 * font_scale_x) + (((cursor_y * font_height + (font_height - 1)) * font_scale_y) << 16),
                 (8 * font_scale_x) + ((1 * font_scale_y) << 16),
                 back_color
        );
      }
    }
    
    blink_ticks = (blink_ticks + 1) % 75;
  }

  return 0;
}

void term_putchar(char c) {
  if (c == '\n' || cursor_x >= width / (8 * font_scale_x)) {
    cursor_x = 0;
    cursor_y++;
    
    if (cursor_y >= height / (font_height * font_scale_y)) {
      cursor_y--;
      
      nop_send(vide, "SCRO", font_height * font_scale_y, 0, 0);
      nop_send(vide, "RECT", (height - (font_height * font_scale_y)) << 16, width + ((font_height * font_scale_y) << 16), back_color);
    }
    
    return;
  }
  
  uint8_t buffer[(bpp >> 3) * 8 * font_scale_x * font_height * font_scale_y];
  size_t pixel_x = cursor_x * 8 * font_scale_x;
  
  for (int y = 0; y < font_height * font_scale_y; y++) {
    size_t index = ((font_height * (c >> 4) + (y / font_scale_y)) << 4) + (c & 0x0F);
    uint8_t byte = font_data[index];
    
    size_t pixel_y = (cursor_y * font_height * font_scale_y) + y;
    size_t offset = (pixel_x + (pixel_y * width)) * (bpp >> 3);
    
    uint8_t line[(bpp >> 3) * 8 * font_scale_x];
    
    for (int x = 0; x < 8; x++) {
      uint32_t color = ((byte >> (7 - x)) & 1) ? fore_color : back_color;

      for (size_t sx = 0; sx < font_scale_x; sx++) {
        memcpy(buffer + (sx + x * font_scale_x) * (bpp >> 3) + y * ((bpp >> 3) * 8 * font_scale_x), &color, (bpp >> 3));
      }
    }
  }
  
  nop_send(vide, "BITM", pixel_x + ((cursor_y * font_height * font_scale_y) << 16), (8 * font_scale_x) + ((font_height * font_scale_y) << 16), (uint32_t)(buffer));
  cursor_x++;
  
  blink_state = 0;
  blink_ticks = 37;
}
