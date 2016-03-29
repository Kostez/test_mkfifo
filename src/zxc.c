#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>

void main {
  char* str;
  int len;
  
  len = read(STDIN, str, 1024);
  write(STDOUT, str, len);
  write(2, str, len);
  
  
}
