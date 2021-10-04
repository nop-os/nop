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
    0x30
  };
  
  syst_index = idt_add(hand);
}

void syst_call(i586_regs_t *regs, idt_hand_t *hand) {
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
      regs->eax = (uint32_t)(syst_list());
      break;
    case SYST_TIME:
      syst_time(regs->ebx, regs->ecx);
      break;
    case SYST_REQU:
      regs->eax = (uint32_t)(syst_requ((void *)(regs->ebx)));
      break;
    case SYST_FREE:
      syst_free((void *)(regs->ebx));
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
  }
}

int syst_load(const char *path) {
  fat_node_t node;
  
  int part = path[0] - '0';
  
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
  if (id > 0 && id <= PROG_MAX) {
    page_free(prog_arr[id - 1].buffer, (prog_arr[id - 1].size + 0x0FFF) >> 12);
    prog_arr[id - 1].free = 1;
  }
}

void syst_paus(int id, int pause) {
  
}

int syst_list(void) {
  
}

void syst_time(int id, int enable) {
  
}

int syst_requ(void *ptr) {
  
}

void syst_free(void *ptr) {
  
}

int syst_open(const char *path) {
  fat_node_t node;
  
  int part = path[0] - '0';
  
  uint32_t cluster = fat_find(part, 0, &node, path + 2);
  if (cluster == 0x0FFFFFFF) return 0;
  
  for (int i = 0; i < SYST_OPEN_MAX; i++) {
    if (syst_files[i].free) {
      strncpy(syst_files[i].path, path, FAT_PATH_MAX - 1);
      syst_files[i].path[FAT_PATH_MAX - 1] = '\0';
      
      syst_files[i].buffer = page_alloc((node.size + 0x01FF) >> 12);
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
  if (id <= 0 || id > SYST_OPEN_MAX) return;
  
  page_free(syst_files[id - 1].buffer, (syst_files[id - 1].size + 0x01FF) >> 12);
  syst_files[id - 1].free = 1;
}

size_t syst_read(int id, void *buffer, size_t size) {
  if (id <= 0 || id > SYST_OPEN_MAX) return 0;
  
  if (size > syst_files[id - 1].size - syst_files[id - 1].offset) {
    size = syst_files[id - 1].size - syst_files[id - 1].offset;
  }
  
  memcpy(buffer, syst_files[id - 1].buffer + syst_files[id - 1].offset, size);
  syst_files[id - 1].offset += size;
  
  return size;
}

size_t syst_writ(int id, void *buffer, size_t size) {
  
}

void syst_seek(int id, size_t offset, int mode) {
  
}

size_t syst_tell(int id) {
  
}

int syst_stat(int id, int new_stat) {
  
}

void syst_size(int id, size_t size) {
  
}

void syst_dele(const char *path) {
  
}
