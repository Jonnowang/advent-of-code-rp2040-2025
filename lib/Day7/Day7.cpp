#include <Arduino.h>
#include <Day7.h>
#include <Day7Data.h>
#include <SharedMemory.h>
#include <Utils.h>

#include <etl/algorithm.h>
#include <etl/map.h>
#include <etl/set.h>
#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

long long solve_day7_part1() {
  struct Data {
    etl::vector<etl::string<150>, 300> data;
    etl::set<int, 150> split_idxs;
    etl::set<int, 150> idx_buffer;
  };
  StaticMemoryBuffer<Data> data_wrapper;
  auto& data = *data_wrapper;

  etl::string<150> buffer;

  long long split_count = 0;

  for (const char *p = day7_data; *p != 0; p++) {
    char c = *p;

    if (c == '\n' && !buffer.empty()) {
      data.data.push_back(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  for (auto i = 0; i < data.data.size(); ++i) {
    // If it is the first row
    if (data.split_idxs.empty()) {
      for (auto j = 0; j < data.data[0].size(); ++j) {
        if (data.data[i][j] == 'S') {
          data.split_idxs.insert(j);
        }
      }
    } else {
      data.idx_buffer.clear();
      for (auto j = data.split_idxs.begin(); j != data.split_idxs.end(); ++j) {
        int j_idx = *j;
        if (data.data[i][j_idx] == '^') {
          data.idx_buffer.insert(j_idx + 1);
          data.idx_buffer.insert(j_idx - 1);
          split_count++;
        } else {
          data.idx_buffer.insert(j_idx);
        }
      }
      data.split_idxs = data.idx_buffer;
    }
  }

  return split_count;
}

long long solve_day7_part2() {
  struct Data {
    etl::vector<etl::string<150>, 300> data;
    etl::map<int, long long, 150> split_idxs;
    etl::map<int, long long, 150> idx_buffer;
  };
  StaticMemoryBuffer<Data> data_wrapper;
  auto& data = *data_wrapper;

  etl::string<150> buffer;

  // Initialize split_idxs map with 0s
  for (int i = 0; i < 150; ++i) {
    data.split_idxs[i] = 0LL;
  }

  long long total_splits = 0;

  for (const char *p = day7_data; *p != 0; p++) {
    char c = *p;

    if (c == '\n' && !buffer.empty()) {
      data.data.push_back(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  for (auto i = 0; i < data.data.size(); ++i) {
    // If it is the first row
    if (i == 0) {
      for (auto j = 0; j < data.data[0].size(); ++j) {
        if (data.data[i][j] == 'S') {
          data.split_idxs[j] = 1;
        }
      }
    } else {
      data.idx_buffer.clear();
      for (int k = 0; k < 150; ++k) {
        data.idx_buffer[k] = 0LL;
      }

      for (const auto &item : data.split_idxs) {
        if (item.second == 0) {
          continue;
        }
        if (data.data[i][item.first] == '^') {
          data.idx_buffer[item.first + 1] += item.second;
          data.idx_buffer[item.first - 1] += item.second;
        } else {
          data.idx_buffer[item.first] += item.second;
        }
      }
      data.split_idxs = data.idx_buffer;
    }
  }

  for (const auto &item : data.split_idxs) {
    total_splits += item.second;
  }

  return total_splits;
}

void solve_day7() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day7_part1();
  print_and_send_solution(7, 1, part1_ans, millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day7_part2();
  print_and_send_solution(7, 2, part2_ans, millis() - start_time);
}
