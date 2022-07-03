#include <ctype.h>

int isspace(int chr) {
  return (!chr || chr == ' ' || chr == '\r' || chr == '\n' || chr == '\t');
}

int isdigit(int chr) {
  return (chr >= '0' && chr <= '9');
}

int toupper(int chr) {
  if (chr >= 'a' && chr <= 'z') return (chr - 'a') + 'A';
  return chr;
}
