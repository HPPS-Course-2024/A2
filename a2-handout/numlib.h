#pragma once

#include <stdint.h>
#include <stdio.h>

// Skip spaces in file.
void skipspaces(FILE *f);

// All functions return 1 on format failure, and EOF if the first read
// is EOF.

int read_uint_ascii(FILE *f, uint32_t *out);
int read_double_ascii(FILE *f, double *out);
int read_uint_le(FILE *f, uint32_t *out);
int read_uint_be(FILE *f, uint32_t *out);
int read_double_bin(FILE *f, double *out);

int write_uint_ascii(FILE *f, uint32_t x);
int write_double_ascii(FILE *f, double x);
int write_uint_le(FILE *f, uint32_t x);
int write_uint_be(FILE *f, uint32_t x);
int write_double_bin(FILE *f, double x);

