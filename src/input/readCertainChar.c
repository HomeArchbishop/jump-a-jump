#include <stdarg.h>
#include <stdlib.h>

#include "../../includes/input/readChar.h"

char readCertainChar (int arg, ...) {
  char* s = (char *)malloc(sizeof(char) * arg);
  int length = 0;
  va_list list;
	va_start(list, arg);
	while(arg > 0) {
		*(s + length) = va_arg(list, int);
    length++;
		arg--;
	}
	va_end(list);

  char c;
  while (1) {
    c = readChar();
    for (int i = 0; i < length; i++) {
      if (c == *(s + i)) {
        free(s);
        return c;
      }
    }
  }
}
