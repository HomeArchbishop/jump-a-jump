#include <stdio.h>

#include <conio.h> // Windows only

char readChar () {
  printf("\33[s"); // save cursor position
  setbuf(stdin, NULL);
  char c = getch();
  printf("\33[u"); // recover cursor position
  return c;
}
