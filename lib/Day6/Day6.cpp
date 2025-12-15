#include <Arduino.h>
#include <Day6.h>
#include <Day6Data.h>
#include <SharedMemory.h>
#include <new>

#include <etl/algorithm.h>
#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

long long solve_day6_part1() {
  struct Data {
    etl::vector<etl::vector<long long, 1500>, 5> numbers;
    etl::vector<long long, 1500> string_buffer;
    etl::vector<char, 1500> operations;
  };
  StaticMemoryBuffer<Data> data_wrapper;
  auto &data = *data_wrapper;

  etl::string<16> buffer;

  long long grand_total = 0;

  bool operation_mode = false;

  for (const char *p = day6_data; *p != 0; p++) {
    char c = *p;

    if (c == '*' || c == '+') {
      operation_mode = true;
    }

    if (c == '\n') {
      if (operation_mode) {
        data.operations.push_back(buffer[0]);
        buffer = "";
      } else {
        data.string_buffer.push_back(etl::to_arithmetic<long long>(buffer));
        buffer = "";
        data.numbers.push_back(data.string_buffer);
        data.string_buffer = {};
      }
    } else if (c == ' ' && buffer.length() == 0) {
      continue;
    } else if (c == ' ') {
      if (operation_mode) {
        data.operations.push_back(buffer[0]);
        buffer = "";
      } else {
        data.string_buffer.push_back(etl::to_arithmetic<long long>(buffer));
        buffer = "";
      }
    } else {
      buffer += c;
    }
  }

  for (auto i = 0; i < data.numbers[0].size(); ++i) {
    if (data.operations[i] == '*') {
      grand_total += data.numbers[0][i] * data.numbers[1][i] *
                     data.numbers[2][i] * data.numbers[3][i];
    } else if (data.operations[i] == '+') {
      grand_total += data.numbers[0][i] + data.numbers[1][i] +
                     data.numbers[2][i] + data.numbers[3][i];
    }
  }

  return grand_total;
}

long long solve_day6_part2() {
  struct Data {
    etl::vector<etl::string<4000>, 4> data;
    etl::vector<char, 1500> operations;
    etl::string<4000> buffer;
  };
  StaticMemoryBuffer<Data> d_wrapper;
  auto &d = *d_wrapper;

  long long grand_total = 0;

  bool operation_mode = false;

  for (const char *p = day6_data; *p != 0; p++) {
    char c = *p;

    if (c == '*' || c == '+') {
      operation_mode = true;
    }

    if (c == '\n' && !d.buffer.empty()) {
      if (operation_mode) {
        d.operations.push_back(d.buffer[0]);
        d.buffer = "";
      } else {
        d.data.push_back(d.buffer);
        d.buffer = "";
      }
    } else if (c == ' ' && operation_mode) {
      if (d.buffer.empty()) {
        continue;
      }
      d.operations.push_back(d.buffer[0]);
      d.buffer = "";
    } else {
      d.buffer += c;
    }
  }

  int op_idx = d.operations.size() - 1;
  etl::vector<long long, 6> ll_buffer;

  for (int i = d.data[0].size() - 1; i >= 0; --i) {
    while (d.data[0][i] != ' ' || d.data[1][i] != ' ' || d.data[2][i] != ' ' ||
           d.data[3][i] != ' ') {
      if (i < 0)
        break;

      etl::string<10> column_str;

      for (size_t r = 0; r < d.data.size(); r++) {
        if (d.data[r][i] != ' ') {
          column_str += d.data[r][i];
        }
      }
      ll_buffer.push_back(etl::to_arithmetic<long long>(column_str));
      i--;
    }

    if (d.operations[op_idx] == '+') {
      grand_total += etl::accumulate(ll_buffer.begin(), ll_buffer.end(), 0LL);
    } else {
      grand_total += etl::accumulate(ll_buffer.begin(), ll_buffer.end(), 1LL,
                                     etl::multiplies<long long>());
    }
    ll_buffer = {};
    op_idx--;
  }

  return grand_total;
}

void solve_day6() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day6_part1();
  Serial.print("Day 6 Part 1 Solution: ");
  Serial.print(part1_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day6_part2();
  Serial.print("Day 6 Part 2 Solution: ");
  Serial.print(part2_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);
}
