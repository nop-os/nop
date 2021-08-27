#ifndef __BOOT_TINYBOOT_H__
#define __BOOT_TINYBOOT_H__

#include <nop/type.h>

typedef struct tb_ent_t tb_ent_t;
typedef struct tb_mem_t tb_mem_t;
typedef struct tb_vid_t tb_vid_t;

struct tb_ent_t {
  uint64_t addr;
  uint64_t size;
  uint32_t type;
} __attribute__((packed));

struct tb_mem_t {
  tb_ent_t *table;
  uint16_t  count;
} __attribute__((packed));

struct tb_vid_t {
  void     *buffer;
  uint16_t  width, height;
  uint16_t  pitch;
  uint8_t   bpp;
} __attribute__((packed));

#endif