#ifndef __CONFIG_H__
#define __CONFIG_H__

void cfg_rewind(int file);
int  cfg_next_str(int file, char *buffer);

int cfg_read_str(int file, const char *name, char *buffer);
int cfg_read_int(int file, const char *name, int *value);

int cfg_read_arr_len(int file, const char *name);
int cfg_read_arr_str(int file, const char *name, int index, char *buffer);
int cfg_read_arr_int(int file, const char *name, int index, int *value);

#endif
