#include <nop/call.h>
#include <stdio.h>

#define TERM_BLACK        "\x1B[30m"
#define TERM_RED          "\x1B[31m"
#define TERM_GREEN        "\x1B[32m"
#define TERM_BROWN        "\x1B[33m"
#define TERM_BLUE         "\x1B[34m"
#define TERM_PURPLE       "\x1B[35m"
#define TERM_CYAN         "\x1B[36m"
#define TERM_LIGHT_GRAY   "\x1B[37m"
#define TERM_GRAY         "\x1B[90m"
#define TERM_LIGHT_RED    "\x1B[91m"
#define TERM_LIGHT_GREEN  "\x1B[92m"
#define TERM_YELLOW       "\x1B[93m"
#define TERM_LIGHT_BLUE   "\x1B[94m"
#define TERM_LIGHT_PURPLE "\x1B[95m"
#define TERM_LIGHT_CYAN   "\x1B[96m"
#define TERM_WHITE        "\x1B[97m"

#define CMD_LENGTH 512

int shell_run(int argc, const char **argv);

int main(int argc, const char **argv) {
  putstr(TERM_WHITE "\nwelcome to nop, the OS for tinkerers!\ntype " TERM_LIGHT_CYAN "help" TERM_WHITE " for a list of commands\n\n");
  
  for (;;) {
    putstr(TERM_LIGHT_GREEN "0:/user " TERM_LIGHT_RED "[0]" TERM_WHITE "> " TERM_LIGHT_CYAN);
    
    char buffer[CMD_LENGTH];
    gets_s(buffer, CMD_LENGTH);
    
    char **new_argv = NULL;
    int new_argc = 0;
    
    new_argv = realloc(new_argv, (new_argc + 1) * sizeof(char *));
    new_argv[new_argc] = NULL;
    
    int offset_1 = 0;
    int offset_2 = 0;
    
    while (buffer[offset_1]) {
      if (isspace(buffer[offset_1])) {
        if (offset_2) {
          new_argc++;
          
          new_argv = realloc(new_argv, (new_argc + 1) * sizeof(char *));
          new_argv[new_argc] = NULL;
          
          offset_2 = 0;
        }
      } else {
        new_argv[new_argc] = realloc(new_argv[new_argc], offset_2 + 2);
        
        new_argv[new_argc][offset_2++] = buffer[offset_1];
        new_argv[new_argc][offset_2] = '\0';
      }
      
      offset_1++;
    }
    
    if (argc) {
      // TODO: change in format string
      int value = shell_run(new_argc, new_argv);
    }
  }
  
  return 1;
}

int shell_run(int argc, const char **argv) {
  if (!strcmp(argv[0], "echo")) {
    
  }
}
