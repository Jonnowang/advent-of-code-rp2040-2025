#include <Arduino.h>
#include <Day6.h>
#include <Day6Data.h>

#include <etl/algorithm.h>
#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

long long solve_day6_part1() {
  etl::vector<etl::vector<long long, 1500>, 5> numbers;
  etl::vector<long long, 1500> string_buffer;
  etl::vector<char, 1500> operations;
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
        operations.push_back(buffer[0]);
        buffer = "";
      } else {
        string_buffer.push_back(etl::to_arithmetic<long long>(buffer));
        buffer = "";
        numbers.push_back(string_buffer);
        string_buffer = {};
      }
    } else if (c == ' ' && buffer.length() == 0) {
      continue;
    } else if (c == ' ') {
      if (operation_mode) {
        operations.push_back(buffer[0]);
        buffer = "";
      } else {
        string_buffer.push_back(etl::to_arithmetic<long long>(buffer));
        buffer = "";
      }
    } else {
      buffer += c;
    }
  }

  for (auto i = 0; i < numbers[0].size(); ++i) {
    if (operations[i] == '*') {
      grand_total +=
          numbers[0][i] * numbers[1][i] * numbers[2][i] * numbers[3][i];
    } else if (operations[i] == '+') {
      grand_total +=
          numbers[0][i] + numbers[1][i] + numbers[2][i] + numbers[3][i];
    }
  }

  return grand_total;
}

long long solve_day6_part2() {
  etl::vector<etl::string<4000>, 4> data;
  etl::vector<char, 1500> operations;
  etl::string<4000> buffer;

  long long grand_total = 0;

  bool operation_mode = false;

  for (const char *p = day6_data; *p != 0; p++) {
    char c = *p;

    if (c == '*' || c == '+') {
      operation_mode = true;
    }

    if (c == '\n' && !buffer.empty()) {
      if (operation_mode) {
        operations.push_back(buffer[0]);
        buffer = "";
      } else {
        data.push_back(buffer);
        buffer = "";
      }
    } else if (c == ' ' && operation_mode) {
      if (buffer.empty()) {
        continue;
      }
      operations.push_back(buffer[0]);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  int op_idx = operations.size() - 1;
  etl::vector<long long, 6> ll_buffer;

  for (int i = data[0].size() - 1; i >= 0; --i) {
    while (data[0][i] != ' ' || data[1][i] != ' ' || data[2][i] != ' ' ||
           data[3][i] != ' ') {
      if (i < 0)
        break;

      etl::string<10> column_str;

      for (size_t r = 0; r < data.size(); r++) {
        if (data[r][i] != ' ') {
          column_str += data[r][i];
        }
      }
      ll_buffer.push_back(etl::to_arithmetic<long long>(column_str));
      i--;
    }

    if (operations[op_idx] == '+') {
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
  const long long part1_ans = solve_day6_part1();
  const long long part2_ans = solve_day6_part2();

  Serial.print("Day 6 Part 1 Solution: ");
  Serial.println(part1_ans);
  Serial.print("Day 6 Part 2 Solution: ");
  Serial.println(part2_ans);
}
