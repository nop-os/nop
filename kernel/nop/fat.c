#include <nop/type.h>
#include <nop/ata.h>
#include <nop/dbg.h>
#include <nop/fat.h>
#include <nop/mem.h>
#include <string.h>

fat_t fat_parts[16];
int fat_count = 0;

static int fat_check(int drive, uint64_t lba) {
  fat_boot_t boot;
  if (!ata_read(drive, lba, &boot, 1)) return 0;
  
  return (boot.boot_signature == 0x28 || boot.boot_signature == 0x29);
}

void fat_init(void) {
  for (int i = 0; i < 4; i++) {
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
            fat_parts[fat_count].drive = i;
            
            dbg_donef("fat: found FAT32 partition\n");
            dbg_infof("- drive:        %d\n", i);
            dbg_infof("- partition:    %d\n", j);
            dbg_infof("- offset:       0x%08X\n", mbr.part[j].lba_addr);
            dbg_infof("- index:        %d\n", fat_count);
            dbg_infof("- fat size:     %d\n", fat_parts[fat_count].boot.table_size);
            dbg_infof("- fat count:    %d\n", fat_parts[fat_count].boot.table_count);
            dbg_infof("- cluster size: %d\n", fat_parts[fat_count].boot.cluster_size);
            dbg_infof("- root cluster: 0x%08X\n", fat_parts[fat_count].boot.root_cluster);
            
            fat_count++;
          }
        }
      }
      
      if (valid_mbr) continue;
      
      if (fat_check(i, 0)) {
        fat_parts[fat_count].boot = *((fat_boot_t *)(&mbr));
        fat_parts[fat_count].start = 0;
        fat_parts[fat_count].drive = i;
        
        dbg_donef("fat: found FAT32 partition\n");
        dbg_infof("- drive:        %d\n", i);
        dbg_infof("- partition:    none\n");
        dbg_infof("- offset:       0x00000000\n");
        dbg_infof("- index:        %d\n", fat_count);
        dbg_infof("- fat size:     %d\n", fat_parts[fat_count].boot.table_size);
        dbg_infof("- fat count:    %d\n", fat_parts[fat_count].boot.table_count);
        dbg_infof("- cluster size: %d\n", fat_parts[fat_count].boot.cluster_size);
        dbg_infof("- root cluster: 0x%08X\n", fat_parts[fat_count].boot.root_cluster);
        
        fat_count++;
      }
    }
  }
}

uint64_t fat_lba(int part, uint32_t cluster) {
  return (uint64_t)(((cluster - 2) * (uint64_t)(fat_parts[part].boot.cluster_size)) + fat_parts[part].boot.reserved + fat_parts[part].boot.table_count * (uint64_t)(fat_parts[part].boot.table_size));
}

uint32_t fat_load_cluster(int part, void *buffer, uint32_t cluster) {
  if (!ata_read(fat_parts[part].drive, fat_lba(part, cluster) + fat_parts[part].start, buffer, fat_parts[part].boot.cluster_size)) return 0x0FFFFFFF;
  
  uint64_t sector = ((uint64_t)(cluster) >> 7) + fat_parts[part].boot.reserved + fat_parts[part].start;
  
  uint32_t fat_buffer[128];
  if (!ata_read(fat_parts[part].drive, sector, fat_buffer, 1)) return 0x0FFFFFFF;
  
  return fat_buffer[cluster & 0x7F];
}

size_t fat_load_chain(int part, void *buffer, uint32_t cluster) {
  size_t count = 0;
  
  while (cluster < 0x0FFFFFF7) {
    cluster = fat_load_cluster(part, buffer, cluster);
    
    count++;
    buffer += 512;
  }
  
  return count;
}

size_t fat_size(int part, uint32_t cluster) {
  uint8_t buffer[512];
  size_t count = 0;
  
  while (cluster < 0x0FFFFFF7) {
    cluster = fat_load_cluster(part, buffer, cluster);
    count++;
  }
  
  return count;
}

uint32_t fat_find(int part, uint32_t directory, fat_node_t *node, const char *path) {
  if (directory >= 0x0FFFFFF7) return 0x0FFFFFFF;
  
  if (!path) return directory;
  if (!strlen(path)) return directory;
  
  if (path[0] == ' ' || path[0] == '/') return fat_find(part, directory, node, ++path);
  if (path[strlen(path) - 1] == ' ' || path[strlen(path) - 1] == '/') return fat_find(part, directory, node, ++path);
  
  if (!directory) return fat_find(part, fat_parts[part].boot.root_cluster, node, path);
  
  char name[12] = "           ";
  int pos = 0;
  
  while (*path) {
    if (*path == '/') break;
    
    if (*path == '.') {
      pos = 8;
    } else if (*path >= 'a' && *path <= 'z') {
      name[pos++] = (*path - 'a') + 'A';
    } else {
      name[pos++] = *path;
    }
    
    path++;
  }
  
  size_t size = (fat_size(part, directory) * fat_parts[part].boot.cluster_size) << 9;
  
  fat_node_t *nodes = mem_alloc(size);
  if (!nodes) return 0x0FFFFFFF;
  
  fat_load_chain(part, nodes, directory);
  size /= sizeof(fat_node_t);
  
  for (size_t i = 0; i < size; i++) {
    if (nodes[i].read_only && nodes[i].hidden && nodes[i].system && nodes[i].volume_id) continue;
    dbg_infof("fat: comparing '%s' with '%s' (%d/%d, 0x%08X)\n", name, nodes[i].name, i, size, ((uint32_t)(nodes[i].cluster_hi) << 16) | (uint32_t)(nodes[i].cluster_lo));
    
    if (!memcmp(name, (char *)(nodes[i].name), 11)) {
      dbg_donef("fat: match found!\n");
      uint32_t cluster = ((uint32_t)(nodes[i].cluster_hi) << 16) | (uint32_t)(nodes[i].cluster_lo);
      
      if (*path == '/' && nodes[i].directory) {
        mem_free(nodes);
        cluster = fat_find(part, cluster, node, path + 1);
      } else {
        if (node) *node = nodes[i];
        mem_free(nodes);
      }
      
      return cluster;
    }
  }
  
  mem_free(nodes);
  return 0x0FFFFFFF;
}
