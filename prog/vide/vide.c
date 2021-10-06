#include <arch/i586.h>
#include <nop/send.h>
#include <nop/type.h>
#include <string.h>

typedef struct tb_vid_t tb_vid_t;

struct tb_vid_t {
  void     *buffer;
  uint16_t  width, height;
  uint16_t  pitch;
  uint8_t   bpp;
} __attribute__((packed));

tb_vid_t *table = NULL;

void vide_write(int offset, void *buffer, int size);
void vide_read(int offset, void *buffer, int size);

void vide_scroll(int lines);
void vide_clear(void);

void vide_rect(int x, int y, int width, int height, uint32_t color);
void vide_bitmap(int x, int y, int width, int height, void *buffer);

__attribute__((__section__(".entry"), __used__))
int nex_start(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  if (type == nop_type("INIT")) {
    table = nop_phys(id, data_1);
    vide_clear();
  } else if (type == nop_type("CLEA")) {
    vide_clear();
  } else if (type == nop_type("SCRO")) {
    vide_scroll(data_1);
  } else if (type == nop_type("RECT")) {
    vide_rect(data_1 & 0xFFFF, (data_1 >> 16) & 0xFFFF, data_2 & 0xFFFF, (data_2 >> 16) & 0xFFFF, data_3);
  } else if (type == nop_type("BITM")) {
    vide_bitmap(data_1 & 0xFFFF, (data_1 >> 16) & 0xFFFF, data_2 & 0xFFFF, (data_2 >> 16) & 0xFFFF, nop_phys(id, data_3));
  } else if (type == nop_type("WRIT")) {
    vide_write(data_1, nop_phys(id, data_2), data_3);
  } else if (type == nop_type("READ")) {
    vide_read(data_1, nop_phys(id, data_2), data_3);
  } else if (type == nop_type("INFO")) {
    uint16_t *width = nop_phys(id, data_1);
    if (width) *width = table->width;
    
    uint16_t *height = nop_phys(id, data_2);
    if (height) *height = table->height;
    
    uint8_t *bpp = nop_phys(id, data_3);
    if (bpp) *bpp = table->bpp;
  }
  
  return 0;
}

void vide_write(int offset, void *buffer, int size) {
  int line = offset / (table->width * (table->bpp >> 3));
  int pixel = offset % (table->width * (table->bpp >> 3));
  
  while (size) {
    int copy_size = size;
    
    if (copy_size > (table->width * (table->bpp >> 3)) - pixel) {
      copy_size = (table->width * (table->bpp >> 3)) - pixel;
    }
    
    memcpy(table->buffer + (table->pitch * line) + pixel, buffer, copy_size);
    pixel = 0;
    
    buffer += copy_size;
    size -= copy_size;
    
    line++;
  }
}

void vide_read(int offset, void *buffer, int size) {
  int line = (int)(offset / (table->width * (table->bpp >> 3)));
  int pixel = (int)(offset % (table->width * (table->bpp >> 3)));
  
  while (size) {
    int copy_size = size;
    
    if (copy_size > (table->width * (table->bpp >> 3)) - pixel) {
      copy_size = (table->width * (table->bpp >> 3)) - pixel;
    }
    
    memcpy(buffer, table->buffer + (table->pitch * line), copy_size);
    
    buffer += copy_size;
    size -= copy_size;
    
    line++;
  }
}

void vide_scroll(int lines) {
  for (int i = 0; i < table->height; i++) {
    int j = (i + lines) % table->height;
    memcpy(table->buffer + (table->pitch * i), table->buffer + (table->pitch * j), table->pitch);
  }
}

void vide_clear(void) {
  memset(table->buffer, 0, table->pitch * table->height);
}

void vide_rect(int x, int y, int width, int height, uint32_t color) {
  if (x >= table->width || y >= table->height) {
    return;
  }
  
  if (x < 0) {
    width += x;
    x = 0;
  }
  
  if (y < 0) {
    height += y;
    y = 0;
  }
  
  if (x + width > table->width) {
    width = table->width - x;
  }
  
  if (y + height > table->height) {
    height = table->height - y;
  }
  
  size_t offset = (size_t)(x) * (table->bpp >> 3) + (size_t)(y) * table->pitch;

  for (int i = 0; i < height; i++) {
    if (table->bpp == 8) {
      i586_rep_stosb(color, width, table->buffer + offset);
    } else if (table->bpp == 16) {
      i586_rep_stosw(color, width, table->buffer + offset);
    } else {
      i586_rep_stosd(color, width, table->buffer + offset);
    }
    
    offset += table->pitch;
  }
}

void vide_bitmap(int x, int y, int width, int height, void *buffer) {
  size_t offset = (size_t)(x) * (table->bpp >> 3) + (size_t)(y) * table->pitch;

  for (int i = 0; i < height; i++) {
    memcpy(table->buffer + offset, buffer + (i * width * (table->bpp >> 3)), width * (table->bpp >> 3));
    offset += table->pitch;
  }
}
