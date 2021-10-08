#include <nop/send.h>
#include <nop/type.h>
#include <string.h>
#include <stdio.h>

char curr_dir[256];
int running = 1;

void cmd_next(char *buffer, const char **cmd);
void cmd_parse(const char *cmd);

__attribute__((__section__(".entry"), __used__))
int nex_start(int id, uint32_t type, uint32_t data_1, uint32_t data_2, uint32_t data_3) {
  if (type != nop_type("INIT")) return 0;
  
  int term = nop_find("TERM");
  if (!term) return 0;
  
  strcpy(curr_dir, "0:/");
  stdio_init(term, term);
  
  printf("SHEL.NEX r01, by segfaultdev\n");
  
  while (running) {
    printf("%s> ", curr_dir);
    
    char buffer[256];
    int length = 0;
    
    for (;;) {
      char c;
      
      while (!nop_send(term, "READ", (uint32_t)(&c), 1, 0));
      
      if (c == '\n') {
        nop_send(term, "WRIT", (uint32_t)(&c), 1, 0);
        break;
      } else if (c == '\b') {
        if (length) {
          nop_send(term, "WRIT", (uint32_t)(&c), 1, 0);
          length--;
        }
      } else if (length < 255) {
        nop_send(term, "WRIT", (uint32_t)(&c), 1, 0);
        buffer[length++] = c;
      }
    }
    
    buffer[length] = '\0';
    cmd_parse(buffer);
  }

  return 0;
}

void cmd_next(char *buffer, const char **cmd) {
  while (**cmd && **cmd != ' ') {
    *(buffer++) = *((*cmd)++);
  }
  
  while (**cmd == ' ') {
    (*cmd)++;
  }
  
  *buffer = '\0';
}

void cmd_parse(const char *cmd) {
  char buffer[64];
  cmd_next(buffer, &cmd);
  
  if (!strcmp(buffer, "echo")) {
    printf("%s\n", cmd);
  } else if (!strcmp(buffer, "pwd")) {
    printf("%s\n", curr_dir);
  } else if (!strcmp(buffer, "clear")) {
    printf("\x1B[2J");
  } else if (!strcmp(buffer, "exit")) {
    running = 0;
  } else if (!strcmp(buffer, "help")) {
    printf("echo pwd clear exit help\n");
  } else {
    int new_id = nop_send(0, "LOAD", (uint32_t)(buffer), 0, 0);
    
    if (!new_id) {
      printf("unknown command: '%s'\n", buffer);
      return;
    }
    
    // TODO: pass arguments(argc, argv) and curr. directory
    nop_send(new_id, "INIT", 0, 0, 0);
    nop_send(0, "KILL", new_id, 0, 0);
  }
}
