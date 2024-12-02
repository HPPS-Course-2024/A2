#include "numlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// avg_doubles.c

int main() {
  double sum   = 0.0;
  int    total = 0;

  double tmp;

  while (1) {
    int res = read_double_bin(stdin, &tmp);
    if (res != 0)
      break;
    sum += tmp;
    total++;
  }

  if (total == 0) {
    return 0;
  }

  double avg = sum / total;
  write_double_ascii(stdout, avg);
  int newline = 10; // ASCII code for newline
  fwrite(&newline, sizeof(int), 1, stdout);
}