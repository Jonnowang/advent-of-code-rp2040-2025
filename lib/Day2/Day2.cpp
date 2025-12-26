#include <Arduino.h>
#include <Day2.h>
#include <Day2Data.h>

#include <algorithm>
#include <etl/string.h>
#include <etl/to_arithmetic.h>

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

long long solve_day2_part1() {
  etl::string<32> buffer;
  etl::string<16> nums[2];
  int idx = 0;
  long long invalid_sum = 0;

  auto compute_invalid = [&]() {
    size_t len_left = strnlen(nums[0].c_str(), 16);
    size_t len_right = strnlen(nums[1].c_str(), 16);

    const size_t size_regime = (std::max(len_left, len_right) / 2) * 2;

    if (len_left > size_regime && len_left == len_right) {
      return;
    }

    if (len_left < size_regime) {
      nums[0] = "1";
      nums[0].append(size_regime - 1, '0');
    } else if (len_right > size_regime) {
      nums[1].assign(size_regime, '9');
    }

    etl::string<8> large_left = nums[0].substr(0, size_regime / 2);
    etl::string<8> large_right = nums[1].substr(0, size_regime / 2);

    etl::string<8> small_left = nums[0].substr(size_regime / 2, size_regime);
    etl::string<8> small_right = nums[1].substr(size_regime / 2, size_regime);

    int large_left_num = atoi(large_left.c_str());
    int large_right_num = atoi(large_right.c_str());

    int small_left_num = atoi(small_left.c_str());
    int small_right_num = atoi(small_right.c_str());

    if (small_left_num > large_left_num) {
      large_left_num++;
    }
    if (small_right_num < large_right_num) {
      large_right_num--;
    }

    for (int i = large_left_num; i <= large_right_num; i++) {
      long long temp = ipow(10, size_regime / 2) * i + i;
      invalid_sum += temp;
    }
  };

  for (const char *p = day2_data; *p != 0; p++) {
    char c = *p;

    if (isDigit(c)) {
      buffer += c;
    } else if (buffer.length() > 0) {
      nums[idx] = buffer;
      idx++;
      buffer = "";

      if (idx == 2) {
        compute_invalid();
        idx = 0;
      }
    }
  }

  return invalid_sum;
}

long long solve_day2_part2() {
  etl::string<32> buffer;
  etl::string<16> nums[2];
  int idx = 0;
  long long invalid_sum = 0;

  auto is_invalid = [](long long n) -> bool {
    char buf[32];
    sprintf(buf, "%lld", n);
    size_t len = strlen(buf);

    for (size_t sub_len = 1; sub_len <= len / 2; sub_len++) {
      if (len % sub_len == 0) {
        bool match = true;
        for (size_t i = sub_len; i < len; i++) {
          if (buf[i] != buf[i % sub_len]) {
            match = false;
            break;
          }
        }
        if (match) return true;
      }
    }
    return false;
  };

  auto compute_invalid = [&]() {
    long long start = atoll(nums[0].c_str());
    long long end = atoll(nums[1].c_str());

    for (long long i = start; i <= end; i++) {
      if (is_invalid(i)) {
        invalid_sum += i;
      }
    }
  };

  for (const char *p = day2_data; *p != 0; p++) {
    char c = *p;

    if (isDigit(c)) {
      buffer += c;
    } else if (buffer.length() > 0) {
      nums[idx] = buffer;
      idx++;
      buffer = "";

      if (idx == 2) {
        compute_invalid();
        idx = 0;
      }
    }
  }

  return invalid_sum;
}

void solve_day2() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day2_part1();
  Serial.print("Day 2 Part 1 Solution: ");
  Serial.print(part1_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day2_part2();
  Serial.print("Day 2 Part 2 Solution: ");
  Serial.print(part2_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);
}
