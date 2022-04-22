#include <nop/alloc.h>
#include <nop/term.h>
#include <nop/type.h>
#include <nop/ata.h>
#include <nop/fat.h>
#include <string.h>

fat_t fat_parts[16];
int fat_count = 0;

static int fat_check(int drive, uint64_t lba) {
  fat_boot_t boot;
  if (!ata_read(drive, lba, &boot, 1)) return 0;
  
  return (boot.boot_signature == 0x28 || boot.boot_signature == 0x29);
}

void fat_init(void) {
  for (int i = 0; i < 9; i++) {
    if (ata_drives[i].present) {
      ata_mbr_t mbr;
      if (!ata_read(i, 0, &mbr, 1)) continue;
      
      int valid_mbr = 0;
      
      for (int j = 0; j < 4; j++) {
        if (mbr.part[j].attr & 0x80 || (mbr.part[j].lba_addr && mbr.part[j].lba_size)) {
          if (fat_check(i, mbr.part[j].lba_addr)) {
            if (!ata_read(i, mbr.part[j].lba_addr, &(fat_parts[fat_count].boot), 1)) continue;
            valid_mbr = 1;
            
            fat_parts[fat_count].start = mbr.part[j].lba_addr;
            fat_parts[fat_count].first = 0;
            fat_parts[fat_count].drive = i;
            
            if (!fat_parts[fat_count].boot.table_size) continue;
            if (!fat_parts[fat_count].boot.cluster_size) continue;
            if (fat_parts[fat_count].boot.root_cluster < 2) continue;
            if (!fat_parts[fat_count].boot.table_count || fat_parts[fat_count].boot.table_count > 2) continue;
            
            term_donef("found FAT32 partition\n");
            term_infof("- drive:        %d\n", i);
            term_infof("- partition:    %d\n", j);
            term_infof("- offset:       0x%08X\n", mbr.part[j].lba_addr);
            term_infof("- index:        %d\n", fat_count);
            term_infof("- fat size:     %d\n", fat_parts[fat_count].boot.table_size);
            term_infof("- fat count:    %d\n", fat_parts[fat_count].boot.table_count);
            term_infof("- cluster size: %d\n", fat_parts[fat_count].boot.cluster_size);
            term_infof("- root cluster: 0x%08X\n", fat_parts[fat_count].boot.root_cluster);
            
            fat_count++;
          }
        }
      }
      
      if (valid_mbr) continue;
      
      if (fat_check(i, 0)) {
        fat_parts[fat_count].boot = *((fat_boot_t *)(&mbr));
        fat_parts[fat_count].start = 0;
        fat_parts[fat_count].first = 0;
        fat_parts[fat_count].drive = i;
        
        if (!fat_parts[fat_count].boot.table_size) continue;
        if (!fat_parts[fat_count].boot.cluster_size) continue;
        if (fat_parts[fat_count].boot.root_cluster < 2) continue;
        if (!fat_parts[fat_count].boot.table_count || fat_parts[fat_count].boot.table_count > 2) continue;
        
        term_donef("found FAT32 partition\n");
        term_infof("- drive:        %d\n", i);
        term_infof("- partition:    none\n");
        term_infof("- offset:       0x00000000\n");
        term_infof("- index:        %d\n", fat_count);
        term_infof("- fat size:     %d\n", fat_parts[fat_count].boot.table_size);
        term_infof("- fat count:    %d\n", fat_parts[fat_count].boot.table_count);
        term_infof("- cluster size: %d\n", fat_parts[fat_count].boot.cluster_size);
        term_infof("- root cluster: 0x%08X\n", fat_parts[fat_count].boot.root_cluster);
        
        fat_count++;
      }
    }
  }
  
  // check where is the kernel located
  uint8_t buffer[512];
  
  for (int i = 0; i < fat_count; i++) {
    if (!ata_read(fat_parts[i].drive, fat_parts[i].start, buffer, 1)) {
      continue;
    } else if (memcmp((void *)(buffer), (void *)(0x00007C00), 512)) {
      continue;
    }
    
    term_infof("booted from partition %d\n", i);
    
    fat_t tmp = fat_parts[0];
    fat_parts[0] = fat_parts[i];
    fat_parts[i] = tmp;
    
    break;
  }
}

uint64_t fat_lba(int part, uint32_t cluster) {
  cluster -= 2;
  return (uint64_t)((cluster * (uint64_t)(fat_parts[part].boot.cluster_size)) + fat_parts[part].boot.reserved + fat_parts[part].boot.table_count * (uint64_t)(fat_parts[part].boot.table_size));
}

int fat_valid(uint32_t cluster) {
  return (cluster < 0x0FFFFFF7);
}

uint32_t fat_next_cluster(int part, uint32_t cluster) {
  uint64_t sector = ((uint64_t)(cluster) >> 7) + fat_parts[part].boot.reserved + fat_parts[part].start;
  
  uint32_t fat_buffer[128];
  if (!ata_read(fat_parts[part].drive, sector, fat_buffer, 1)) return 0x0FFFFFFF;
  
  return fat_buffer[cluster & 0x7F];
}

uint32_t fat_load_cluster(int part, void *buffer, uint32_t cluster) {
  if (!ata_read(fat_parts[part].drive, fat_lba(part, cluster) + fat_parts[part].start, buffer, fat_parts[part].boot.cluster_size)) return 0x0FFFFFFF;
  return fat_next_cluster(part, cluster);
}

size_t fat_load_chain(int part, void *buffer, uint32_t cluster) {
  size_t count = 0;
  
  while (cluster >= 2 && cluster < 0x0FFFFFF7) {
    cluster = fat_load_cluster(part, buffer, cluster);
    
    count++;
    buffer += 512;
  }
  
  return count;
}

size_t fat_size(int part, uint32_t cluster) {
  uint8_t buffer[512];
  size_t count = 0;
  
  while (cluster >= 2 && cluster < 0x0FFFFFF7) {
    cluster = fat_load_cluster(part, buffer, cluster);
    count++;
  }
  
  return count;
}

uint32_t fat_find(int part, uint32_t directory, uint32_t *parent, size_t *index, fat_node_t *node, const char *path) {
  if (directory >= 0x0FFFFFF7) return 0x0FFFFFFF;
  if (!directory) directory = fat_parts[part].boot.root_cluster;
  
  if (parent) *parent = directory;
  
  if (!path) {
    node->size = (fat_size(part, directory) * fat_parts[part].boot.cluster_size) << 9;
    node->directory = 1;
    
    return directory;
  }
  
  if (!strlen(path)) {
    node->size = (fat_size(part, directory) * fat_parts[part].boot.cluster_size) << 9;
    node->directory = 1;
    
    return directory;
  }
  
  if (path[0] == ' ' || path[0] == '/') return fat_find(part, directory, parent, index, node, ++path);
  
  char name[12] = "           ";
  int pos = 0;
  
  while (*path) {
    if (pos == 11) break;
    if (*path == '/') break;
    
    if (*path == '.') {
      if (pos < 8) {
        pos = 8;
      }
    } else if (*path >= 'a' && *path <= 'z') {
      name[pos++] = (*path - 'a') + 'A';
    } else {
      name[pos++] = *path;
    }
    
    path++;
  }
  
  size_t size = (fat_size(part, directory) * fat_parts[part].boot.cluster_size) << 9;
  fat_node_t *nodes = malloc(size);
  
  if (!nodes) {
    term_failf("not enough free heap space, returning\n");
    return 0x0FFFFFFF;
  }
  
  fat_load_chain(part, nodes, directory);
  size /= sizeof(fat_node_t);
  
  for (size_t i = 0; i < size; i++) {
    if (nodes[i].read_only && nodes[i].hidden && nodes[i].system && nodes[i].volume_id) continue;
    if (!nodes[i].name[0] || nodes[i].name[0] == 0xE5) continue;
    
    char buffer[12] = {0};
    memcpy(buffer, nodes[i].name, 11);
    
    term_infof("comparing '%s' with '%s'(%d/%d, 0x%08X)\n", name, buffer, i, size, ((uint32_t)(nodes[i].cluster_hi) << 16) | (uint32_t)(nodes[i].cluster_lo));
    
    if (!memcmp(name, (char *)(nodes[i].name), 11)) {
      term_donef("match found!\n");
      uint32_t cluster = ((uint32_t)(nodes[i].cluster_hi) << 16) | (uint32_t)(nodes[i].cluster_lo);
      
      if (*path == '/' && nodes[i].directory) {
        free(nodes);
        cluster = fat_find(part, cluster, parent, index, node, path + 1);
      } else {
        if (index) {
          *index = i;
        }
        
        if (node) {
          *node = nodes[i];
          
          if (nodes[i].directory) {
            node->size = (fat_size(part, cluster) * fat_parts[part].boot.cluster_size) << 9;
          }
        }
        
        free(nodes);
      }
      
      return cluster;
    }
  }
  
  free(nodes);
  
  term_warnf("match not found\n");
  return 0x0FFFFFFF;
}

void fat_mark_cluster(int part, uint32_t curr, uint32_t next) {
  uint32_t fat_buffer[128];
  uint64_t sector = ((uint64_t)(curr) >> 7) + fat_parts[part].boot.reserved + fat_parts[part].start;
  
  if (fat_parts[part].first > (uint64_t)(curr) >> 7) {
    fat_parts[part].first = (uint64_t)(curr) >> 7;
  }
  
  if (!ata_read(fat_parts[part].drive, sector, fat_buffer, 1)) {
    return;
  }
  
  fat_buffer[curr & 0x7F] = next;
  
  if (!ata_write(fat_parts[part].drive, sector, fat_buffer, 1)) {
    return;
  }
}

uint32_t fat_requ_cluster(int part) {
  uint32_t fat_buffer[128];
  
  for (uint64_t i = fat_parts[part].first; i < fat_parts[part].boot.table_size; i++) {
    uint64_t sector = i + fat_parts[part].boot.reserved + fat_parts[part].start;
    
    if (!ata_read(fat_parts[part].drive, sector, fat_buffer, 1)) {
      return 0x0FFFFFFF;
    }
    
    for (int j = 0; j < 128; j++) {
      if (!fat_buffer[j]) {
        uint32_t cluster = ((i << 7) | j) & 0x0FFFFFFF;
        fat_parts[part].first = i;
        
        term_infof("found free cluster 0x%08X\n", cluster);
        return cluster;
      }
    }
  }
  
  return 0x0FFFFFFF;
}

uint32_t fat_requ_chain(int part, size_t size) {
  size_t cluster_size = fat_parts[part].boot.cluster_size << 9;
  size = (size + (cluster_size - 1)) / cluster_size;
  
  uint32_t start = 0;
  
  uint32_t prev = 0;
  uint32_t curr = 0;
  
  while (size) {
    curr = fat_requ_cluster(part);
    
    if (prev) fat_mark_cluster(part, prev, curr);
    fat_mark_cluster(part, curr, 0x0FFFFFFF);
    
    if (!start) start = curr;
    
    prev = curr;
    size--;
  }
  
  return start;
}

void fat_free_chain(int part, uint32_t cluster) {
  if (!cluster) return;
  
  for (;;) {
    uint32_t next = fat_next_cluster(part, cluster);
    if (!next || next >= 0x0FFFFFF7) return;
    
    fat_mark_cluster(part, cluster, 0);
    cluster = next;
  }
}

uint32_t fat_edit_cluster(int part, void *buffer, uint32_t cluster) {
  if (!ata_write(fat_parts[part].drive, fat_lba(part, cluster) + fat_parts[part].start, buffer, fat_parts[part].boot.cluster_size)) return 0x0FFFFFFF;
  return fat_next_cluster(part, cluster);
}

void fat_edit_chain(int part, void *buffer, uint32_t cluster) {
  while (cluster >= 2 && cluster < 0x0FFFFFF7) {
    cluster = fat_edit_cluster(part, buffer, cluster);
    buffer += 512;
  }
}

size_t fat_get_free(int part, uint32_t directory) {
  if (!directory) directory = fat_parts[part].boot.root_cluster;
  size_t size = (fat_size(part, directory) * fat_parts[part].boot.cluster_size) << 9;
  
  fat_node_t *nodes = malloc(size);
  if (!nodes) return 0xFFFFFFFF;
  
  fat_load_chain(part, nodes, directory);
  size /= sizeof(fat_node_t);
  
  for (int i = 0; i < size; i++) {
    if (!nodes[i].name[0] || nodes[i].name[0] == 0xE5) {
      free(nodes);
      return i;
    }
  }
  
  free(nodes);
  return 0xFFFFFFFF;
}

void fat_set_node(int part, uint32_t directory, size_t index, fat_node_t node) {
  if (!directory) directory = fat_parts[part].boot.root_cluster;
  size_t size = (fat_size(part, directory) * fat_parts[part].boot.cluster_size) << 9;
  
  fat_node_t *nodes = malloc(size);
  if (!nodes) return;
  
  fat_load_chain(part, nodes, directory);
  size /= sizeof(fat_node_t);
  
  if (index < size) {
    nodes[index] = node;
    fat_edit_chain(part, nodes, directory);
  }
  
  free(nodes);
}
