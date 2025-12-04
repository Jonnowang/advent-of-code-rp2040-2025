#include <Arduino.h>
#include <Day4.h>
#include <Day4Data.h>

#include <etl/algorithm.h>
#include <etl/string.h>
#include <etl/vector.h>

int solve_day4_part1() {
  etl::vector<etl::string<136>, 136> roll_array;
  etl::string<136> buffer;

  int movable_rolls = 0;

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

int solve_day4_part2() { return 0; }

void solve_day4() {
  const int part1_ans = solve_day4_part1();
  const int part2_ans = solve_day4_part2();

  Serial.print("Day 4 Part 1 Solution: ");
  Serial.println(part1_ans);
  Serial.print("Day 4 Part 2 Solution: ");
  Serial.println(part2_ans);
}
