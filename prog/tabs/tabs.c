#include <nop/call.h>
#include <nop/prog.h>
#include <nop/term.h>
#include <nop/type.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tabs.h>

tab_t *tabs = NULL;
int tab_count = 0, tab_index = 0;

int term_width = 0, term_height = 0;

int tab_alloc(const char *title, int prog) {
  tabs = realloc(tabs, (tab_count + 1) * sizeof(tab_t));
  
  strcpy(tabs[tab_count].title, title);
  
  tabs[tab_count].cursor_x = 0;
  tabs[tab_count].cursor_y = 0;
  
  tabs[tab_count].screen = malloc(term_width * (term_height - 1) * sizeof(char_t));
  
  tabs[tab_count].write_head = 0;
  tabs[tab_count].read_head = 0;
  
  tabs[tab_count].prog = prog;
  
  return tab_count++;
}

void tab_free(int index) {
  free(tabs[index].screen);
  
  memmove(tabs + index, tabs + index + 1, ((tab_count - 1) - index) * sizeof(tab_t));
  tab_count--;
  
  if (tab_index >= tab_count) tab_index = tab_count;
}

void tab_write(int index, char chr) {
  
}

size_t $term_write(const char *buffer, size_t size) {
  int prog_id = call_getid();
  int tab_id = -1;
  
  for (int i = 0; i < tab_count; i++) {
    if (tabs[i].prog = prog_id) {
      tab_id = i;
      break;
    }
  }
  
  if (tab_id < 0) return 0;
  
  uint8_t new_buffer[size];
  call_read(new_buffer, buffer, size);
  
  for (size_t i = 0; i < size; i++) {
    tab_write(tab_id, new_buffer[i]);
  }
  
  return size;
}

size_t $term_read(char *buffer, size_t size) {
  int prog_id = call_getid();
  int tab_id = -1;
  
  for (int i = 0; i < tab_count; i++) {
    if (tabs[i].prog = prog_id) {
      tab_id = i;
      break;
    }
  }
  
  if (tab_id < 0) return 0;
  size_t count = 0;
  
  while (size > 0 && tabs[tab_id].read_head != tabs[tab_id].write_head) {
    call_write(buffer++, tabs[tab_id].queue + tabs[tab_id].read_head++, 1);
    size--;
    
    tabs[tab_id].read_head &= 63;
    count++;
  }
  
  return count;
}

int main(int argc, const char **argv) {
  uint32_t term_size = term_getsize();
  
  term_width  = (term_size >> 0 ) & 0xFFFF;
  term_height = (term_size >> 16) & 0xFFFF;
  
  call_t tabs_calls[] = {
    (call_t){"term_write", 0, (void *)($term_write)},
    (call_t){"term_read", 0, (void *)($term_read)},
  };
  
  tab_alloc(argv[1], prog_load(argv[1], argv + 1, environ, tabs_calls, 2));
  
  while (tab_count > 0) {
    for (int i = 0; i < tab_count; i++) {
      if (prog_check(tabs[i].prog) < 2) {
        prog_kill(tabs[i].prog);
        tab_free(i);
        
        i--;
      }
    }
    
    uint8_t temp;
    
    while (term_read(&temp, 1)) {
      tabs[tab_index].queue[tabs[tab_index].write_head++] = temp;
      tabs[tab_index].write_head &= 63;
    }
    
    prog_skip();
  }
  
  return 1;
}
