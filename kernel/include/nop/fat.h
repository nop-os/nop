#ifndef __NOP_FAT_H__
#define __NOP_FAT_H__

#define FAT_PATH_MAX 256

#include <nop/type.h>

typedef struct fat_boot_t fat_boot_t;
typedef struct fat_t fat_t;

typedef struct fat_node_t fat_node_t;

struct fat_boot_t {
  uint8_t jump[3];
  uint8_t oem[8];
  
  uint16_t sector_size;
  uint8_t  cluster_size;
  uint16_t reserved;
  
  uint8_t  table_count;
  uint16_t entry_count;
  
  uint16_t sector_count_16;
  
  uint8_t media_type;
  
  uint16_t table_size_16;
  
  uint16_t track_size;
  uint16_t head_count;
  
  uint32_t hidden_size;
  uint32_t sector_count;

  uint32_t table_size;
  
  uint16_t flags;
  uint16_t fat32_version;
  
  uint32_t root_cluster;
  
  uint16_t fat32_info;
  uint16_t backup_sector;
  
  uint8_t padding_1[12];
  
  uint8_t drive_id;
  uint8_t padding_2[1];
  
  uint8_t  boot_signature;
  uint32_t part_id;
  uint8_t  part_name[11];
  uint8_t  fat32_name[8];
  
  uint8_t boot_code[420];
  uint16_t part_signature;
} __attribute__((packed));

struct fat_t {
  fat_boot_t boot;
  
  uint64_t start;
  uint64_t first;
  
  int drive;
};

struct fat_node_t {
  uint8_t name[11];
  
  uint8_t read_only : 1;
  uint8_t hidden    : 1;
  uint8_t system    : 1;
  uint8_t volume_id : 1;
  uint8_t directory : 1;
  uint8_t archive   : 1;
  uint8_t reserved_1: 2;
  
  uint8_t reserved_2;
  
  uint8_t  c_ticks;
  uint16_t c_hour:   5;
  uint16_t c_minute: 6;
  uint16_t c_second: 5;
  uint16_t c_year  : 7;
  uint16_t c_month : 4;
  uint16_t c_day   : 5;
  
  uint16_t a_year  : 7;
  uint16_t a_month : 4;
  uint16_t a_day   : 5;
  
  uint16_t cluster_hi;
  
  uint16_t m_hour:   5;
  uint16_t m_minute: 6;
  uint16_t m_second: 5;
  uint16_t m_year  : 7;
  uint16_t m_month : 4;
  uint16_t m_day   : 5;
  
  uint16_t cluster_lo;
  uint32_t size;
} __attribute__((packed));

extern fat_t fat_parts[];
extern int fat_count;

void fat_init(void);

uint64_t fat_lba(int part, uint32_t cluster);
int      fat_valid(uint32_t cluster);

// read stuff

uint32_t fat_next_cluster(int part, uint32_t cluster);

uint32_t fat_load_cluster(int part, void *buffer, uint32_t cluster);
size_t   fat_load_chain(int part, void *buffer, uint32_t cluster);
size_t   fat_size(int part, uint32_t cluster);

uint32_t fat_find(int part, uint32_t directory, uint32_t *parent, size_t *index, fat_node_t *node, const char *path);

// write stuff

void     fat_mark_cluster(int part, uint32_t curr, uint32_t next); // writes to the FAT
uint32_t fat_requ_cluster(int part); // allocates a single cluster, doesnt change anything

uint32_t fat_requ_chain(int part, size_t size); // size in bytes, automatically converted, changes the FAT
void     fat_free_chain(int part, uint32_t cluster);

uint32_t fat_edit_cluster(int part, void *buffer, uint32_t cluster);
void     fat_edit_chain(int part, void *buffer, uint32_t cluster);

size_t fat_get_free(int part, uint32_t directory);
void   fat_set_node(int part, uint32_t directory, size_t index, fat_node_t node);

#endif
