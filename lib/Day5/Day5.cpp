#include <Arduino.h>
#include <Day5.h>
#include <Day5Data.h>
#include <SharedMemory.h>

#include <etl/algorithm.h>
#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

int solve_day5_part1() {
  struct Data {
    etl::vector<etl::pair<long long, long long>, 200> intervals;
    etl::vector<long long, 1000> ingredients;
  };
  StaticMemoryBuffer<Data> data_wrapper;
  auto &data = *data_wrapper;

  etl::string<32> buffer;
  etl::pair<long long, long long> interval_buf = {};

  int fresh_count = 0;

  bool finished_constructing_intervals = false;

  for (const char *p = day5_data; *p != 0; p++) {
    char c = *p;

    if (finished_constructing_intervals) {
      if (c == '\n') {
        data.ingredients.push_back(etl::to_arithmetic<long long>(buffer));
        buffer = "";
      } else {
        buffer += c;
      }
      continue;
    }

    if (c == '\n') {
      interval_buf.second = etl::to_arithmetic<long long>(buffer);
      data.intervals.push_back(interval_buf);

      if (p[1] == '\n') {
        finished_constructing_intervals = true;
      }
      interval_buf = {};
      buffer = "";
    } else if (c == '-') {
      interval_buf.first = etl::to_arithmetic<long long>(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  // etl::sort(intervals.begin(), intervals.end());
  for (const auto &item : data.ingredients) {
    for (const auto &interval : data.intervals) {
      if (item >= interval.first && item <= interval.second) {
        fresh_count++;
        break;
      }
    }
  }

  return fresh_count;
}

long long solve_day5_part2() {
  using Intervals = etl::vector<etl::pair<long long, long long>, 200>;
  StaticMemoryBuffer<Intervals> intervals_wrapper;
  auto &intervals = *intervals_wrapper;

  etl::string<32> buffer;
  etl::pair<long long, long long> interval_buf = {};

  long long fresh_count = 0;

  for (const char *p = day5_data; *p != 0; p++) {
    char c = *p;

    if (c == '\n') {
      interval_buf.second = etl::to_arithmetic<long long>(buffer);
      intervals.push_back(interval_buf);

      if (p[1] == '\n') {
        break;
      }
      interval_buf = {};
      buffer = "";
    } else if (c == '-') {
      interval_buf.first = etl::to_arithmetic<long long>(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  etl::sort(intervals.begin(), intervals.end());
  long long left_p = intervals[0].first;
  long long right_p = intervals[0].second;
  for (auto interval = intervals.begin() + 1; interval != intervals.end();
       ++interval) {
    if (interval[0].second <= right_p) {
      continue;
    } else if (interval[0].first <= right_p + 1) {
      right_p = interval[0].second;
    } else {
      fresh_count += right_p - left_p + 1;
      left_p = interval[0].first;
      right_p = interval[0].second;
    }
  }
  fresh_count += right_p - left_p + 1;

  return fresh_count;
}

void solve_day5() {
  unsigned long start_time = millis();
  int part1_ans = solve_day5_part1();
  Serial.print("Day 5 Part 1 Solution: ");
  Serial.print(part1_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day5_part2();
  Serial.print("Day 5 Part 2 Solution: ");
  Serial.print(part2_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);
}
