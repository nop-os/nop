#include <exec/aout.h>
#include <nop/file.h>
#include <nop/page.h>
#include <nop/prog.h>
#include <nop/term.h>
#include <nop/type.h>
#include <string.h>

int aout_check(int file) {
  file_seek(file, 0, SEEK_SET);
  
  aout_t aout;
  if (!file_read(file, &aout, sizeof(aout_t))) return 0;
  
  if (AOUT_MAGIC(aout) != 0x0107 || AOUT_TARGET(aout)) return 0;
  return 1;
}

prog_data_t aout_load(int file) {
  file_seek(file, 0, SEEK_SET);
  
  aout_t aout;
  file_read(file, &aout, sizeof(aout_t));
  
  prog_data_t data;
  data.size = aout.text + aout.data + aout.bss;
  
  data.data = page_alloc((data.size + 0x0FFF) >> 12, 0);
  file_read(file, data.data, aout.text + aout.data);
  
  memset(data.data + (aout.text + aout.data), 0, aout.bss);
  data.entry = (void *)(aout.entry);
  
  return data;
}
