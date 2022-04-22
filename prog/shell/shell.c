#include <nop/file.h>
#include <nop/call.h>
#include <nop/prog.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

int main(int argc, const char **argv, const char **envp) {
  putstr(TERM_WHITE "\nwelcome to nop, the toy OS you never asked for!\ntype " TERM_LIGHT_CYAN "help" TERM_WHITE " for a list of commands\n\n");
  int value = 1;
  
  for (;;) {
    if (value == 1) printf(TERM_LIGHT_GREEN "%s" TERM_WHITE "> " TERM_LIGHT_CYAN, getenv("PATH"));
    else printf(TERM_LIGHT_GREEN "%s " TERM_LIGHT_RED "[%d]" TERM_WHITE "> " TERM_LIGHT_CYAN, getenv("PATH"), value);
    
    char buffer[CMD_LENGTH];
    gets_s(buffer, CMD_LENGTH);
    
    putstr(TERM_WHITE);
    
    char **new_argv = NULL;
    int new_argc = 0;
    
    new_argv = realloc(new_argv, (new_argc + 1) * sizeof(char *));
    new_argv[new_argc] = NULL;
    
    int offset_1 = 0;
    int offset_2 = 0;
    
    do {
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
      
    } while (buffer[offset_1++]);
    
    for (int i = 0; i < new_argc; i++) {
      if (new_argv[i][0] == '$') {
        const char *env = getenv(new_argv[i] + 1);
        free(new_argv[i]);
        
        if (env) {
          new_argv[i] = strdup(env);
        } else {
          new_argv[i] = strdup("");
        }
      }
    }
    
    if (new_argc) {
      value = shell_run(new_argc, new_argv);
    }
    
    for (int i = 0; i < new_argc; i++) {
      free(new_argv[i]);
    }
    
    free(new_argv);
  }
  
  return 1;
}

int shell_run(int argc, const char **argv) {
  if (!strcmp(argv[0], "go")) {
    if (argc != 2) {
      printf("go: invalid argument(s)\n");
      return 0;
    }
    
    char new_path[FILE_PATH_MAX];
    
    if (strlen(argv[1]) >= 2 && argv[1][0] >= '0' && argv[1][0] <= '9' && argv[1][1] == ':') {
      strcpy(new_path, argv[1]);
    } else {
      strcpy(new_path, getenv("PATH"));
      strcat(new_path, "/");
      strcat(new_path, argv[1]);
    }
    
    int length = strlen(new_path);
    
    if (new_path[length - 1] == '/') {
      // you psych
      new_path[length - 1] = '\0';
    }
    
    int file = file_open(new_path);
    
    if (!file) {
      printf("go: cannot open '%s'\n", new_path);
      return 0;
    }
    
    if (!(file_getmode(file) & FILE_DIRECTORY)) {
      printf("go: '%s' not a directory\n", new_path);
      
      file_close(file, 0);
      return 0;
    }
    
    file_close(file, 0);
    
    setenv("PATH", new_path, 1);
    return 1;
  } else if (!strcmp(argv[0], "back")) {
    if (argc != 1) {
      printf("back: invalid argument(s)\n");
      return 0;
    }
    
    char new_path[FILE_PATH_MAX];
    strcpy(new_path, getenv("PATH"));
    
    char *last = strrchr(new_path, '/');
    if (last) *last = '\0';
    
    setenv("PATH", new_path, 1);
    return 1;
  } else if (!strcmp(argv[0], "clear")) {
    if (argc != 1) {
      printf("clear: invalid argument(s)\n");
      return 0;
    }
    
    putstr("\x1B[2J");
    return 1;
  } else if (!strcmp(argv[0], "exit")) {
    exit(1);
  } else if (!strcmp(argv[0], "set")) {
    if (argc == 2) {
      unsetenv(argv[1]);
    } else if (argc == 3) {
      setenv(argv[1], argv[2], 1);
    } else {
      printf("set: invalid argument(s)\n");
      return 0;
    }
    
    return 1;
  } else if (argc == 1 && strlen(argv[0]) == 2 && argv[0][0] >= '0' && argv[0][0] <= '9' && argv[0][1] == ':') {
    if (1) {
      printf("shell: cannot open '%s'\n", argv[0]);
      return 0;
    }
    
    setenv("PATH", argv[0], 1);
  }
  
  int prog = prog_load(argv[0], argv, environ, NULL, 0);
  
  if (!prog) {
    printf("%s: program not found\n", argv[0]);
    return 0;
  }
  
  int value = prog_wait(prog);
  prog_kill(prog);
  
  return value;
}
