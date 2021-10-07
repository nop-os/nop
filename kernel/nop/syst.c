#include <arch/i586.h>
#include <nop/page.h>
#include <nop/prog.h>
#include <nop/syst.h>
#include <nop/type.h>
#include <nop/virt.h>
#include <nop/dbg.h>
#include <nop/fat.h>
#include <nop/mem.h>
#include <string.h>

syst_file_t *syst_files = NULL;
size_t syst_index = 0;

void syst_init(void) {
  syst_files = mem_alloc(sizeof(syst_file_t) * SYST_OPEN_MAX);
  
  if (!syst_files) {
    dbg_failf("syst: cannot allocate file table\n");
    dbg_panic();
  }
  
  for (int i = 0; i < SYST_OPEN_MAX; i++) {
    syst_files[i].free = 1;
  }
  
  idt_hand_t hand = (idt_hand_t){
    syst_call,
    NULL,
    IDT_NOP_BASE
  };
  
  syst_index = idt_add(hand);
}

void syst_call(i586_regs_t *regs, idt_hand_t *hand) {
  if (hand->id != IDT_NOP_BASE) {
    prog_call((int)(hand->data), PROG_TRIG, (uint32_t)(regs), hand->id, 0);
    return;
  }
  
  if (regs->edi) {
    regs->eax = prog_call(regs->edi, regs->eax, regs->ebx, regs->ecx, regs->edx);
    return;  
  }
  
  switch (regs->eax) {
    case SYST_LOAD:
      regs->eax = (uint32_t)(syst_load((void *)(regs->ebx)));
      break;
    case SYST_KILL:
      syst_kill(regs->ebx);
      break;
    case SYST_PAUS:
      syst_paus(regs->ebx, regs->ecx);
      break;
    case SYST_LIST:
      regs->eax = (uint32_t)(syst_list(regs->ebx, (void *)(regs->ecx), (void *)(regs->edx)));
      break;
    case SYST_TIME:
      syst_time(regs->ebx, regs->ecx);
      break;
    case SYST_REQU:
      regs->eax = (uint32_t)(syst_requ(regs->ebx));
      break;
    case SYST_FREE:
      syst_free((void *)(regs->ebx), regs->ecx);
      break;
    case SYST_OPEN:
      regs->eax = (uint32_t)(syst_open((void *)(regs->ebx)));
      break;
    case SYST_CLOS:
      syst_clos(regs->ebx);
      break;
    case SYST_READ:
      regs->eax = (uint32_t)(syst_read(regs->ebx, (void *)(regs->ecx), regs->edx));
      break;
    case SYST_WRIT:
      regs->eax = (uint32_t)(syst_writ(regs->ebx, (void *)(regs->ecx), regs->edx));
      break;
    case SYST_SEEK:
      syst_seek(regs->ebx, regs->ecx, regs->edx);
      break;
    case SYST_TELL:
      regs->eax = (uint32_t)(syst_tell(regs->ebx));
      break;
    case SYST_STAT:
      regs->eax = (uint32_t)(syst_stat(regs->ebx, regs->ecx));
      break;
    case SYST_SIZE:
      syst_size(regs->ebx, regs->ecx);
      break;
    case SYST_DELE:
      syst_dele((void *)(regs->ebx));
      break;
    case SYST_PHYS:
      regs->eax = (uint32_t)(syst_phys(regs->ebx, (void *)(regs->ecx)));
      break;
    case SYST_HOOK:
      regs->eax = (uint32_t)(syst_hook(regs->ebx, (void *)(regs->ecx)));
      break;
    case SYST_RELE:
      syst_rele(regs->ebx);
      break;
  }
}

int syst_load(const char *path) {
  fat_node_t node;
  
  int part = path[0] - '0';
  dbg_infof("syst: %d: load program '%s'\n", prog_id, path);
  
  uint32_t cluster = fat_find(part, 0, &node, path + 2);
  if (cluster == 0x0FFFFFFF) return 0;
  
  for (int i = 0; i < PROG_MAX; i++) {
    if (prog_arr[i].free) {
      size_t size = (fat_size(part, cluster) * fat_parts[part].boot.cluster_size) << 9;
      
      prog_arr[i].buffer = (void *)(page_alloc((size + 0x0FFF) >> 12));
      prog_arr[i].size = size;
      prog_arr[i].free = 0;
      prog_arr[i].tick = 0;
      
      if (!prog_arr[i].buffer) {
        dbg_failf("syst: cannot allocate space for program: '%s'\n", path);
        dbg_panic();
      }
      
      fat_load_chain(part, prog_arr[i].buffer, cluster);
      uint32_t *nex_header = prog_arr[i].buffer;
      
      if (nex_header[0] != 0x2158454E) {
        dbg_failf("syst: file is not a NEX program: '%s'\n", path);
        dbg_panic();
      }
      
      prog_arr[i].start = (void *)(VIRT_NOP_PROG + 8);
      memcpy(prog_arr[i].name, nex_header + 1, 4);
      
      return i + 1;
    }
  }
  
  return 0;
}

void syst_kill(int id) {
  dbg_infof("syst: %d: kill program %d\n", prog_id, id);
  
  if (id > 0 && id <= PROG_MAX) {
    page_free(prog_arr[id - 1].buffer, (prog_arr[id - 1].size + 0x0FFF) >> 12);
    prog_arr[id - 1].free = 1;
    
    for (int i = 0; i < PROG_MAX; i++) {
      if (!prog_arr[i].free && prog_arr[i].tick) {
        prog_call(i + 1, PROG_DEAD, id, 0, 0);
      }
    }
  }
}

void syst_paus(int id, int pause) {
  // TODO: implement PAUS
  
  dbg_failf("syst: %d: PAUS not implemented\n", prog_id);
  dbg_panic();
}

int syst_list(int id, char *name, size_t *size) {
  if (id <= 0 || id > PROG_MAX) {
    id = prog_id;
  }
  
  dbg_infof("syst: %d: listing %d\n", prog_id, id);
  
  if (name) {
    memcpy(name, prog_arr[id - 1].name, 4);
    ((char *)(name))[4] = '\0';
  }
  
  if (size) {
    *size = (size_t)(prog_arr[id - 1].size);
  }
  
  if (id == PROG_MAX) {
    return 0;
  } else if (prog_arr[id].free) {
    return 0;
  } else {
    return id + 1;
  }
}

void syst_time(int id, int enable) {
  if (id <= 0 || id > PROG_MAX) {
    id = prog_id;
  }
  
  dbg_infof("syst: %d: set timer of %d to %d\n", prog_id, id, enable);
  prog_arr[id - 1].tick = enable;
}

void *syst_requ(size_t count) {
  dbg_infof("syst: %d: allocate %d pages\n", prog_id, count);
  
  if (!count) return NULL;
  return page_alloc(count);
}

void syst_free(void *ptr, size_t count) {
  dbg_infof("syst: %d: free %d pages at 0x%08X\n", prog_id, count, ptr);
  page_free(ptr, count);
}

int syst_open(const char *path) {
  fat_node_t node;
  
  int part = path[0] - '0';
  dbg_infof("syst: %d: open file '%s'\n", prog_id, path);
  
  uint32_t cluster = fat_find(part, 0, &node, path + 2);
  if (cluster == 0x0FFFFFFF) return 0;
  
  for (int i = 0; i < SYST_OPEN_MAX; i++) {
    if (syst_files[i].free) {
      strncpy(syst_files[i].path, path, FAT_PATH_MAX - 1);
      syst_files[i].path[FAT_PATH_MAX - 1] = '\0';
      
      syst_files[i].buffer = page_alloc((node.size + 0x0FFF) >> 12);
      syst_files[i].size = node.size;
      syst_files[i].offset = 0;
      syst_files[i].free = 0;
      
      if (!syst_files[i].buffer) {
        dbg_failf("syst: cannot allocate space for file: '%s'\n", path);
        dbg_panic();
      }
      
      fat_load_chain(part, syst_files[i].buffer, cluster);
      syst_files[i].stat = (node.read_only * SYST_RO) | (node.hidden * SYST_HD) | (node.system * SYST_SY) | (node.directory * SYST_DR);
      
      return i + 1;
    }
  }
  
  return 0;
}

void syst_clos(int id) {
  dbg_infof("syst: %d: close file %d\n", prog_id, id);
  
  if (id <= 0 || id > SYST_OPEN_MAX) return;
  if (syst_files[id - 1].free) return;
  
  page_free(syst_files[id - 1].buffer, (syst_files[id - 1].size + 0x01FF) >> 12);
  syst_files[id - 1].free = 1;
}

size_t syst_read(int id, void *buffer, size_t size) {
  dbg_infof("syst: %d: read %d bytes from file %d to 0x%08X\n", prog_id, size, id, buffer);
  
  if (id <= 0 || id > SYST_OPEN_MAX) return 0;
  if (syst_files[id - 1].free) return 0;
  
  if (size > syst_files[id - 1].size - syst_files[id - 1].offset) {
    size = syst_files[id - 1].size - syst_files[id - 1].offset;
  }
  
  memcpy(buffer, syst_files[id - 1].buffer + syst_files[id - 1].offset, size);
  syst_files[id - 1].offset += size;
  
  return size;
}

size_t syst_writ(int id, void *buffer, size_t size) {
  // TODO: implement WRIT
  
  dbg_failf("syst: %d: WRIT not implemented\n", prog_id);
  dbg_panic();
  
  return 0; // does nothing!
}

void syst_seek(int id, size_t offset, int mode) {
  dbg_infof("syst: %d: seek file %d to offset %d, mode %d\n", prog_id, id, offset, mode);
  
  if (id <= 0 || id > SYST_OPEN_MAX) return;
  if (syst_files[id - 1].free) return;
  
  if (mode == SYST_SEEK_ZERO) {
    syst_files[id - 1].offset = offset;
  } else if (mode == SYST_SEEK_OFFS) {
    syst_files[id - 1].offset += offset;
  } else {
    syst_files[id - 1].offset = syst_files[id - 1].size - offset;
  }
}

size_t syst_tell(int id) {
  dbg_infof("syst: %d: tell offset of file %d\n", prog_id, id);
  
  if (id <= 0 || id > SYST_OPEN_MAX) return 0;
  if (syst_files[id - 1].free) return 0;
  
  return syst_files[id - 1].offset;
}

int syst_stat(int id, int new_stat) {
  if (new_stat == -1) {
    dbg_infof("syst: %d: get stat of file %d\n", prog_id, id);
  } else {
    dbg_infof("syst: %d: set stat of file %d to %d\n", prog_id, id, new_stat);
  }
  
  if (id <= 0 || id > SYST_OPEN_MAX) return 0;
  if (syst_files[id - 1].free) return 0;
  
  if (new_stat != -1) {
    syst_files[id - 1].stat = new_stat;
  }
  
  return syst_files[id - 1].stat;
}

void syst_size(int id, size_t size) {
  // TODO: implement SIZE
  
  dbg_failf("syst: %d: SIZE not implemented\n", prog_id);
  dbg_panic();
}

void syst_dele(const char *path) {
  // TODO: implement DELE
  
  dbg_failf("syst: %d: DELE not implemented\n", prog_id);
  dbg_panic();
}

void *syst_phys(int id, void *addr) {
  if (id <= 0 || id > PROG_MAX) {
    id = prog_id;
  }
  
  dbg_infof("syst: %d: converted program %d address 0x%08X to 0x%08X\n", prog_id, id, addr, ((uint32_t)(addr) - VIRT_NOP_PROG) + (uint32_t)(prog_arr[id - 1].buffer));
  return (void *)(((uint32_t)(addr) - VIRT_NOP_PROG) + (uint32_t)(prog_arr[id - 1].buffer));
}

int syst_hook(int id, void *func) {
  idt_hand_t hand = (idt_hand_t){
    syst_call,
    (void *)(prog_id),
    id
  };
  
  dbg_infof("syst: %d: hooked handler for %d\n", prog_id, id);
  
  size_t val = idt_add(hand);
  if (val == 0xFFFFFFFF) return 0;
  
  return (int)(val) + 1;
}

void syst_rele(int id) {
  dbg_infof("syst: %d: released handler %d\n", prog_id, id);
  
  if (!id) return;
  idt_remove((size_t)(id - 1));
}
