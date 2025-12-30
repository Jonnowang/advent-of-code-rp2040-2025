#include <Arduino.h>
#include <Day3.h>
#include <Day3Data.h>
#include <Utils.h>

#include <etl/algorithm.h>
#include <etl/string.h>
#include <etl/vector.h>

static long long ipow(int base, int exp) {
  long long result = 1;
  while (exp > 0) {
    if (exp % 2 == 1)
      result *= base;

    base *= base;
    exp /= 2;
  }
  return result;
}

long long solve_day3_part1() {
  etl::string<101> buffer;
  long long total_joltage = 0;

  for (const char *p = day3_data; *p != 0; p++) {
    char c = *p;

    if (c == '\n') {
      int largest = 0;
      int second_largest = 0;

      for (auto it = buffer.rbegin(); it != buffer.rend(); ++it) {
        const char cur = *it;
        if (cur >= '0' && cur <= '9') {
          int numeric_value = cur - '0';

          if (numeric_value >= largest) {
            if (largest >= second_largest) {
              second_largest = largest;
            }
            largest = numeric_value;
          } else if (second_largest == 0 && largest > 0) {
            second_largest = largest;
            largest = numeric_value;
          }
        }
      }

      total_joltage += largest * 10 + second_largest;
      buffer = "";
    } else {
      buffer += c;
    }
  }

  return total_joltage;
}

long long solve_day3_part2() {
  etl::string<101> buffer;
  long long total_joltage = 0;

  for (const char *p = day3_data; *p != 0; p++) {
    char c = *p;

    if (c == '\n') {
      etl::vector<int, 12> largest;
      if (buffer.size() >= 12) {
        etl::transform(buffer.end() - 12, buffer.end(),
                       etl::back_inserter(largest),
                       [](char c) { return c - '0'; });
      }

      // Skip last 12 since we already pre-initialised the largest vector
      for (auto it = buffer.rbegin() + 12; it != buffer.rend(); ++it) {
        const char cur = *it;
        if (cur >= '0' && cur <= '9') {
          int current_value = cur - '0';

          for (auto l = largest.begin(); l != largest.end(); ++l) {
            if (current_value < *l) {
              break;
            }
            int temp = current_value;
            current_value = *l;
            *l = temp;
          }
        }
      }

      for (auto i = 0; i != largest.size(); ++i) {
        total_joltage += largest[i] * ipow(10, 11 - i);
      }
      buffer = "";
    } else {
      buffer += c;
    }
  }

  return total_joltage;
}

void solve_day3() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day3_part1();
  print_and_send_solution(3, 1, part1_ans, millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day3_part2();
  print_and_send_solution(3, 2, part2_ans, millis() - start_time);
}
