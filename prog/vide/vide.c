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

void vide_write(size_t offset, void *buffer, size_t size);
void vide_clear(void);

__attribute__((__section__(".entry"), __used__))
int nex_start(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  if (type == 0x54494E49) { // INIT
    table = (tb_vid_t *)(data_1);
    vide_clear();
  } else if (type == 0x41454C43) { // CLEA
    vide_clear();
  } else if (type == 0x54495257) { // WRIT
    vide_write((size_t)(data_1), (void *)(data_2), (size_t)(data_3));
  }
  
  return 0;
}

void vide_write(size_t offset, void *buffer, size_t size) {
  int line = (int)(offset / (table->width * (table->bpp >> 8)));
  int pixel = (int)(offset % (table->width * (table->bpp >> 8)));
  
  while (size) {
    int copy_size = size;
    
    if (copy_size > (table->width * (table->bpp >> 8)) - pixel) {
      copy_size = (table->width * (table->bpp >> 8)) - pixel;
    }
    
    memcpy(table->buffer + (table->pitch * line), buffer, copy_size);
    
    buffer += copy_size;
    size -= copy_size;
    
    line++;
  }
}

void vide_clear(void) {
  for (int i = 0; i < table->height; i++) {
    memset(table->buffer, 0, table->pitch * table->height);
  }
}
