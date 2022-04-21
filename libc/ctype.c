#include <ctype.h>

int isspace(int chr) {
  return (!chr || chr == ' ' || chr == '\n' || chr == '\t');
}
