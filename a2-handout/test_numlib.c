#include "numlib.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void test_write_double_bin(double x) {
  FILE* f = fopen("data.bin", "wb");
  if (f == NULL) {
    perror("Failed to open file");
    exit(1);
  }
  write_double_bin(f, x);
  fclose(f);
}

void test_read_double_bin(double* buf) {
  FILE* f = fopen("data.bin", "rb");
  if (f == NULL) {
    perror("Failed to open file");
    exit(1);
  }
  read_double_bin(f, buf);
  fclose(f);
}

void test_write_double_ascii(double x) {
  FILE* f = fopen("data.txt", "w");
  if (f == NULL) {
    perror("Failed to open file");
    exit(1);
  }
  write_double_ascii(f, x);
  fclose(f);
}

void test_read_double_ascii(double* buf) {
  FILE* f = fopen("data.txt", "r");
  if (f == NULL) {
    perror("Failed to open file");
    exit(1);
  }
  read_double_ascii(f, buf);
  fclose(f);
}

int main(void) {
  // 
  test_write_double_bin(-154);
  // write_double(154.69);
  // double x;
  // read_double(&x);
  // printf("%f\n", x);
  test_write_double_ascii(-154);
  // double x;
  // test_read_double_ascii(&x);
  // printf("%f\n", x);
  return 0;
}