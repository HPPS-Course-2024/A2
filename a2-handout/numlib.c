#include "numlib.h"
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BYTE_MASK 0xFF

#define YELLOW "\033[38;2;255;255;85m"
#define RESET "\033[0m"
#define GRAY "\033[38;2;140;140;140m"
#define DEBUG_FLAG 1

#if DEBUG_FLAG
#define LOG(level, fmt, ...)                                                   \
  do {                                                                         \
    time_t     rawtime;                                                        \
    struct tm* timeinfo;                                                       \
    char       timestr[20];                                                    \
    time(&rawtime);                                                            \
    timeinfo = localtime(&rawtime);                                            \
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", timeinfo);         \
    fprintf(stderr, "[%s%s%s] [%s:%d in %s] %s%s%s: " fmt "\n", GRAY, timestr, \
            RESET, __FILE__, __LINE__, __func__, YELLOW, level, RESET,         \
            ##__VA_ARGS__);                                                    \
  } while (0)
#else
#define LOG(level, fmt, ...)                                                   \
  do {                                                                         \
  } while (0)
#endif

#define LOG_INFO(fmt, ...) LOG("INFO", fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG("ERROR", fmt, ##__VA_ARGS__)

void skipspaces(FILE* f) {
  while (1) {
    int c = fgetc(f);
    if (!isspace(c)) {
      if (c != EOF) {
        ungetc(c, f);
      }
      return;
    }
  }
}

int read_uint_ascii(FILE* f, uint32_t* out) {
  int      read = 0;
  uint32_t num  = 0;

  while (1) {
    int c = fgetc(f);

    if (c >= '0' && c <= '9') {
      num = num * 10 + (c - '0');
    } else {
      if (c != EOF) {
        ungetc(c, f);
      }
      if (read == 0) {
        if (c == EOF) {
          return EOF;
        } else {
          return 1;
        }
      } else {
        *out = num;
        return 0;
      }
    }
    read++;
  }
}

int read_double_ascii(FILE* f, double* out) {
  (void)f;
  (void)out;
  return 0;
}

int read_uint_le(FILE* f, uint32_t* out) {
  int b0, b1, b2, b3;
  b0 = fgetc(f);

  if (b0 == EOF) {
    return EOF;
  }

  b1 = fgetc(f);
  b2 = fgetc(f);
  b3 = fgetc(f);

  if (b1 == EOF || b2 == EOF || b3 == EOF) {
    return 1;
  }

  *out = ((uint32_t)b0) | ((uint32_t)b1 << 8) | ((uint32_t)b2 << 16) |
         ((uint32_t)b3 << 24);
  LOG_INFO("Storing in out (le): %x", *out);
  return 0;
}

int read_uint_be(FILE* f, uint32_t* out) {
  int byte0, byte1, byte2, byte3; // Has to be int as EOF is -1

  byte0 = fgetc(f);
  if (byte0 == EOF) {
    return EOF;
  }

  byte1 = fgetc(f);
  byte2 = fgetc(f);
  byte3 = fgetc(f);

  if (byte1 == EOF || byte2 == EOF || byte3 == EOF) {
    return 1;
  }
  *out = ((uint32_t)byte0) << 24 | ((uint32_t)byte1 << 16) |
         ((uint32_t)byte2 << 8) | ((uint32_t)byte3);
  LOG_INFO("Storing in out (be): %x", *out);
  return 0;
}

int read_double_bin(FILE* f, double* out) {
  (void)f;
  (void)out;
  assert(0);
}

int write_uint_ascii(FILE* f, uint32_t x) {
  if (fprintf(f, "%u", x) < 0) {
    return 1;
  } else {
    return 0;
  }
}

int write_double_ascii(FILE* f, double x) {
  (void)f;
  (void)x;
  assert(0);
}

int write_uint_le(FILE* f, uint32_t x) {
  fputc(x >> 0, f);
  fputc(x >> 8, f);
  fputc(x >> 16, f);
  fputc(x >> 24, f);
  return 0;
}

int write_uint_be(FILE* f, uint32_t x) {
  // First store the msbyte ... to ... lsbyte
  for (int i = 0; i < 4; i++) {
    if (fputc((x >> (3 - i) * 8) & BYTE_MASK, f) == EOF) {
      LOG_ERROR("%s", "fputc failed for byte");
      return 1;
    }
  }
  return 0;
}

int write_double_bin(FILE* f, double x) {
  (void)f;
  (void)x;
  assert(0);
}

/* int read_uint_be_alternative(FILE* f, uint32_t* out) {
  unsigned char bytes[4] = {0};
  for (int i = 0; i < 4; i++) {
    int result = fgetc(f);
    bytes[i]   = (unsigned char)result;
    if (result == EOF) {
      LOG_ERROR("End of file error");
      return 1;
    }
  }
  for (int i = 3; i >= 0; i--) {
    *out |= (bytes[3 - i] << 8 * i);
  }
  return 0;
} */

/* int write_uint_be_alternative(FILE* f, uint32_t x) {
  // First store the msbyte ... to ... lsbyte
  if (fputc(x >> 24, f) == EOF) {
    LOG_ERROR("fputc failed", "");
    return 1;
  }
  if (fputc(x >> 16, f)) {
    LOG_ERROR("fputc failed", "");
    return 1;
  }
  if (fputc(x >> 8, f)) {
    LOG_ERROR("fputc failed", "");
    return 1;
  }
  if (fputc(x >> 0, f)) {
    LOG_ERROR("fputc failed", "");
    return 1;
  }
  return 0;
} */