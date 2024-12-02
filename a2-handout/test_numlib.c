#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

#define CHECKMARK "\u2714"
#define CROSSMARK "\u2718"

void print_header() {
  printf("\n%s%-3s %-25s | %-14s %-14s %s%s\n", BOLD, "No", "Description",
         "Expected", "Got", "Result", RESET);
}

typedef int (*Comparator)(void* expected, void* got);
typedef void (*Printer)(void* value, char* buffer, size_t buffer_size);

typedef struct {
  int passed;
  int failed;
} TestResult;

int assert_test(uint8_t test_case, const char* description, void* expected,
                void* got, Comparator compare, Printer print_expected,
                Printer print_got, TestResult* result) {
  const char* result_icon;
  const char* result_color;
  int         succeeded = compare(expected, got);

  if (succeeded) {
    result_icon  = CHECKMARK;
    result_color = GREEN;
    result->passed++;
  } else {
    result_icon  = CROSSMARK;
    result_color = RED;
    result->failed++;
  }

  char expected_str[32];
  char got_str[32];

  print_expected(expected, expected_str, sizeof(expected_str));
  print_got(got, got_str, sizeof(got_str));

  char parenthesized_description[40];
  snprintf(parenthesized_description, sizeof(parenthesized_description), "(%s)",
           description);

  printf("%-3u %-25s | %-14s %-14s %s%s%s\n", test_case,
         parenthesized_description, expected_str, got_str, result_color,
         result_icon, RESET);
  return !succeeded;
}

int test_read_uint_be(int* counter, TestResult* result) {
  int      success  = 0;
  int      value    = 0x12345678;
  uint32_t expected = 0x87654321;
  uint32_t actual   = 0;

  FILE* f = fmemopen(&value, sizeof(value), "r");
  int   r = read_uint_be(f, &actual);
  fclose(f);

  success |= assert_test((*counter)++, "Read 0x12345678", &expected, &actual,
                         &uint32_uint32_comparator, &to_uint32_base16,
                         &to_uint32_base16, result);

  ;
}

int main() {
  struct {
    int (*func_ptr)(int*, TestResult*);
    const char* name;
  } test_cases[] = {};

  int        test_counter = 1;
  TestResult result       = {0, 0};

  for (int i = 0; i < (int)(sizeof(test_cases) / sizeof(test_cases[0])); i++) {
    print_header();
    printf("%s:\n", test_cases[i].name);

    int   res    = test_cases[i].func_ptr(&test_counter, &result);
    char* color  = res == 0 ? GREEN : RED;
    char* result = res == 0 ? "PASSED" : "FAILED";

    printf("%s %s%s%s:\n\n", test_cases[i].name, color, result, RESET);

    char msg_buffer[100];
    snprintf(msg_buffer, sizeof(msg_buffer), "Test failed for %s",
             test_cases[i].name);
    ASSERT(res == 0, msg_buffer);
  }
  printf("Summary: ");
  printf("%s%d tests passed%s, ", GREEN, result.passed, RESET);
  printf("%s%d tests failed%s\n\n", RED, result.failed, RESET);
}