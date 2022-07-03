#include <nop/call.h>
#include <nop/prog.h>
#include <nop/term.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

size_t $term_write(const char *buffer, size_t size) {
  uint8_t new_buffer[size];
  call_read(new_buffer, buffer, size);
  
  return term_write(new_buffer, size);
}

size_t $term_read(char *buffer, size_t size) {
  uint8_t new_buffer[size];
  
  size_t new_size = term_read(buffer, size);
  call_write(buffer, new_buffer, size);
  
  return new_size;
}

int main(int argc, const char **argv) {
  call_t term_calls[] = {
    (call_t){"term_write", 0, (void *)($term_write)},
    (call_t){"term_read", 0, (void *)($term_read)},
  };
  
  int prog = prog_load(argv[1], argv + 1, environ, term_calls, 2);
  prog_wait(prog);
  
  prog_kill(prog);
  return 1;
}
