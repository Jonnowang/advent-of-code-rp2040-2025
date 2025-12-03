#include <Arduino.h>
#include <Day3.h>
#include <Day3Data.h>

#include <etl/string.h>

int solve_day3_part1() {
  etl::string<101> buffer;
  int total_joltage = 0;

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

int solve_day3_part2() { return 0; }

void solve_day3() {
  const int part1_ans = solve_day3_part1();
  Serial.print("Day 3 Part 1 Solution: ");
  Serial.println(part1_ans);
}
