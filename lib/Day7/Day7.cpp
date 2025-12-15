#include <Arduino.h>
#include <Day7.h>
#include <Day7Data.h>
#include <SharedMemory.h>
#include <new>

#include <etl/algorithm.h>
#include <etl/map.h>
#include <etl/set.h>
#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

int solve_day7_part1() {
  using DataVec = etl::vector<etl::string<150>, 300>;
  StaticMemoryBuffer<DataVec> data_wrapper;
  auto& data = *data_wrapper;

  etl::string<150> buffer;
  etl::set<int, 150> split_idxs;

  int split_count = 0;

  for (const char *p = day7_data; *p != 0; p++) {
    char c = *p;

    if (c == '\n' && !buffer.empty()) {
      data.push_back(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  for (auto i = 0; i < data.size(); ++i) {
    // If it is the first row
    if (split_idxs.empty()) {
      for (auto j = 0; j < data[0].size(); ++j) {
        if (data[i][j] == 'S') {
          split_idxs.insert(j);
        }
      }
    } else {
      etl::set<int, 150> idx_buffer;
      for (auto j = split_idxs.begin(); j != split_idxs.end(); ++j) {
        int j_idx = *j;
        if (data[i][j_idx] == '^') {
          idx_buffer.insert(j_idx + 1);
          idx_buffer.insert(j_idx - 1);
          split_count++;
        } else {
          idx_buffer.insert(j_idx);
        }
      }
      split_idxs = idx_buffer;
    }
  }

  return split_count;
}

long long solve_day7_part2() {
  using DataVec = etl::vector<etl::string<150>, 300>;
  StaticMemoryBuffer<DataVec> data_wrapper;
  auto& data = *data_wrapper;

  etl::string<150> buffer;
  etl::map<int, long long, 150> split_idxs;
  for (int i = 0; i < 150; ++i) {
    split_idxs[i] = 0LL;
  }

  long long total_splits = 0;

  for (const char *p = day7_data; *p != 0; p++) {
    char c = *p;

    if (c == '\n' && !buffer.empty()) {
      data.push_back(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  for (auto i = 0; i < data.size(); ++i) {
    // If it is the first row
    if (i == 0) {
      for (auto j = 0; j < data[0].size(); ++j) {
        if (data[i][j] == 'S') {
          split_idxs[j] = 1;
        }
      }
    } else {
      etl::map<int, long long, 150> idx_buffer;
      for (int i = 0; i < 150; ++i) {
        idx_buffer[i] = 0LL;
      }
      for (const auto &item : split_idxs) {
        if (item.second == 0) {
          continue;
        }
        if (data[i][item.first] == '^') {
          idx_buffer[item.first + 1] += item.second;
          idx_buffer[item.first - 1] += item.second;
        } else {
          idx_buffer[item.first] += item.second;
        }
      }
      split_idxs = idx_buffer;
    }
  }

  for (const auto &item : split_idxs) {
    total_splits += item.second;
  }

  return total_splits;
}

void solve_day7() {
  unsigned long start_time = millis();
  int part1_ans = solve_day7_part1();
  Serial.print("Day 7 Part 1 Solution: ");
  Serial.print(part1_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day7_part2();
  Serial.print("Day 7 Part 2 Solution: ");
  Serial.print(part2_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);
}
