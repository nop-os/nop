#ifndef __TABS_H__
#define __TABS_H__

typedef struct char_t char_t;
typedef struct tab_t tab_t;

struct char_t {
  char value;
  int invert;
};

struct tab_t {
  char title[64];
  
  int cursor_x, cursor_y;
  char_t *screen;
  
  uint8_t queue[64];
  int write_head, read_head;
  
  int prog;
};

extern tab_t *tabs;
extern int tab_count, tab_index;

extern int term_width, term_height;

int  tab_alloc(const char *title, int prog);
void tab_free(int index);

void tab_write(int index, char chr);

#endif
