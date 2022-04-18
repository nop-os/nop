#include <nop/call.h>
#include <nop/prog.h>
#include <nop/term.h>
#include <nop/type.h>
#include <nop/virt.h>

size_t $term_write(const char *buffer, size_t size) {
  for (size_t left = size; left; left--) {
    // TODO: only call virt_phys on page boundaries
    char c = *((const char *)(virt_phys(prog_list[prog_id - 1].table, (void *)(buffer))));
    
    term_putchr(c);
    buffer++;
  }
  
  return size;
}
