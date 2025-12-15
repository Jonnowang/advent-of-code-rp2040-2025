#include <Arduino.h>
#include <Day1.h>
#include <Day1Data.h>

int solve_day1_part1() {
  const char *ptr = day1_data;

  String buffer = "";
  int zero_count = 0;
  int dial_position = 50;
  bool is_add;

  auto update_dial = [&]() {
    const int buf_as_int = buffer.toInt();
    if (is_add) {
      dial_position += buf_as_int;
    } else {
      dial_position -= buf_as_int;
    }

    dial_position = ((dial_position % 100) + 100) % 100;
    if (dial_position == 0) {
      zero_count++;
    }
    buffer = "";
  };

  while (*ptr) {
    if (*ptr == 'R') {
      is_add = true;
    } else if (*ptr == 'L') {
      is_add = false;
    } else if (*ptr == '\n') {
      update_dial();
    } else {
      buffer += *ptr;
    }
    ptr++;
  }

  if (buffer.length() > 0) {
    update_dial();
  }

  return zero_count;
}

int solve_day1_part2() {
  const char *ptr = day1_data;

  String buffer = "";
  int zero_count = 0;
  int dial_position = 50;
  bool is_add;

  auto update_dial = [&]() {
    const int buf_as_int = buffer.toInt();
    if (is_add) {
      dial_position += buf_as_int;
      zero_count += dial_position / 100;
    } else {
      dial_position -= buf_as_int;
      zero_count -= dial_position / 100;
      if (dial_position <= 0 && (dial_position + buf_as_int != 0)) {
        zero_count++;
      }
    }
    dial_position = ((dial_position % 100) + 100) % 100;
    buffer = "";
  };

  while (*ptr) {
    if (*ptr == 'R') {
      is_add = true;
    } else if (*ptr == 'L') {
      is_add = false;
    } else if (*ptr == '\n') {
      update_dial();
    } else {
      buffer += *ptr;
    }
    ptr++;
  }

  if (buffer.length() > 0) {
    update_dial();
  }

  return zero_count;
}

void solve_day1() {
  unsigned long start_time = millis();
  int part1_solution = solve_day1_part1();
  Serial.print("Day 1 Part 1 Solution: ");
  Serial.print(part1_solution);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);

  start_time = millis();
  int part2_solution = solve_day1_part2();
  Serial.print("Day 1 Part 2 Solution: ");
  Serial.print(part2_solution);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);
}
