#include <Arduino.h>
#include <Day4.h>
#include <Day4Data.h>
#include <SharedMemory.h>
#include <Utils.h>

#include <etl/algorithm.h>
#include <etl/string.h>
#include <etl/vector.h>

long long solve_day4_part1() {
  using RollArray = etl::vector<etl::string<136>, 136>;
  StaticMemoryBuffer<RollArray> roll_array_wrapper;
  auto &roll_array = *roll_array_wrapper;
  etl::string<136> buffer;

  long long movable_rolls = 0;

  // Construct matrix
  for (const char *p = day4_data; *p != 0; p++) {
    char c = *p;

    if (c == '\n') {
      roll_array.push_back(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  int row_cnt = roll_array.size();
  int col_cnt = roll_array[0].size();

  for (int i = 0; i < row_cnt; ++i) {
    for (int j = 0; j < col_cnt; ++j) {
      if (roll_array[i][j] == '.') {
        continue;
      }

      int cnt_adj_rolls = 0;
      for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
          if (dx == 0 && dy == 0)
            continue;

          int new_i = i + dx;
          int new_j = j + dy;
          if (new_i < 0 || new_j < 0 || new_i >= row_cnt || new_j >= col_cnt)
            continue;

          if (roll_array[new_i][new_j] == '@') {
            cnt_adj_rolls++;
          }
        }
      }

      if (cnt_adj_rolls < 4) {
        movable_rolls++;
      }
    }
  }

  return movable_rolls;
}

long long solve_day4_part2() {
  using RollArray = etl::vector<etl::string<136>, 136>;
  StaticMemoryBuffer<RollArray> roll_array_wrapper;
  auto &roll_array = *roll_array_wrapper;
  etl::string<136> buffer;

  long long movable_rolls = 0;

  // Construct matrix
  for (const char *p = day4_data; *p != 0; p++) {
    char c = *p;

    if (c == '\n') {
      roll_array.push_back(buffer);
      buffer = "";
    } else {
      buffer += c;
    }
  }

  int row_cnt = roll_array.size();
  int col_cnt = roll_array[0].size();

  int moved = 1;

  while (moved > 0) {
    moved = 0;
    for (int i = 0; i < row_cnt; ++i) {
      for (int j = 0; j < col_cnt; ++j) {
        if (roll_array[i][j] == '.') {
          continue;
        }

        int cnt_adj_rolls = 0;
        for (int dx = -1; dx <= 1; ++dx) {
          for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0)
              continue;

            int new_i = i + dx;
            int new_j = j + dy;
            if (new_i < 0 || new_j < 0 || new_i >= row_cnt || new_j >= col_cnt)
              continue;

            if (roll_array[new_i][new_j] == '@') {
              cnt_adj_rolls++;
            }
          }
        }

        if (cnt_adj_rolls < 4) {
          movable_rolls++;
          moved++;
          // Technically should wait until full loop to remove but it's fine
          // since it would only apply to things which are removeable in the
          // next loop anyways
          roll_array[i][j] = '.';
        }
      }
    }
  }

  return movable_rolls;
}

void solve_day4() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day4_part1();
  print_and_send_solution(4, 1, part1_ans, millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day4_part2();
  print_and_send_solution(4, 2, part2_ans, millis() - start_time);
}
