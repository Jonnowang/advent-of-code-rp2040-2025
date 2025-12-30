#include <Arduino.h>
#include <Day9.h>
#include <Day9Data.h>
#include <SharedMemory.h>
#include <Utils.h>

#include <etl/algorithm.h>
#include <etl/map.h>
#include <etl/set.h>
#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

struct Tile {
  long long x, y;
};

long long solve_day9_part1() {
  using TileVec = etl::vector<Tile, 500>;
  StaticMemoryBuffer<TileVec> tile_vec_wrapper;
  auto &tiles = *tile_vec_wrapper;

  etl::string<10> num_str;
  etl::vector<long long, 2> coords;

  for (const char *p = day9_data; *p; p++) {
    if (*p >= '0' && *p <= '9') {
      num_str += *p;
    } else {
      coords.push_back(etl::to_arithmetic<long long>(num_str));
      num_str.clear();
      if (*p == '\n' || coords.full()) {
        tiles.push_back({coords[0], coords[1]});
        coords.clear();
      }
    }
  }

  long long max_area = 0;
  for (int i = 0; i < tiles.size(); i++) {
    for (int j = i + 1; j < tiles.size(); j++) {
      long long area = (abs(tiles[i].x - tiles[j].x) + 1) *
                       (abs(tiles[i].y - tiles[j].y) + 1);
      if (area > max_area) {
        max_area = area;
      }
    }
  }

  return max_area;
}

long long solve_day9_part2() {
  using TileVec = etl::vector<Tile, 500>;
  StaticMemoryBuffer<TileVec> tile_vec_wrapper;
  auto &tiles = *tile_vec_wrapper;

  etl::string<10> num_str;
  etl::vector<long long, 2> coords;

  for (const char *p = day9_data; *p; p++) {
    if (*p >= '0' && *p <= '9') {
      num_str += *p;
    } else {
      coords.push_back(etl::to_arithmetic<long long>(num_str));
      num_str.clear();
      if (*p == '\n' || coords.full()) {
        tiles.push_back({coords[0], coords[1]});
        coords.clear();
      }
    }
  }

  long long max_area = 0;
  size_t n = tiles.size();
  // Helpers
  auto min_pair = [](long long a, long long b) { return a < b ? a : b; };
  auto max_pair = [](long long a, long long b) { return a > b ? a : b; };

  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      // Construct bounding box to test
      long long x1 = tiles[i].x;
      long long y1 = tiles[i].y;
      long long x2 = tiles[j].x;
      long long y2 = tiles[j].y;

      long long xmin = min_pair(x1, x2);
      long long xmax = max_pair(x1, x2);
      long long ymin = min_pair(y1, y2);
      long long ymax = max_pair(y1, y2);

      long long area = (xmax - xmin + 1) * (ymax - ymin + 1);
      if (area <= max_area)
        continue;

      bool invalid = false;
      for (size_t k = 0; k < n; k++) {
        long long ex1 = tiles[k].x;
        long long ey1 = tiles[k].y;

        long long ex2 = tiles[(k + 1) % n].x;
        long long ey2 = tiles[(k + 1) % n].y;

        if (ex1 == ex2) { // Vertical Edge
          if (ex1 > xmin && ex1 < xmax) {
            long long ey_lo = min_pair(ey1, ey2);
            long long ey_hi = max_pair(ey1, ey2);
            if (max_pair(ey_lo, ymin) < min_pair(ey_hi, ymax)) {
              invalid = true;
              break;
            }
          }

        } else { // Horizontal Edge
          if (ey1 > ymin && ey1 < ymax) {
            long long ex_lo = min_pair(ex1, ex2);
            long long ex_hi = max_pair(ex1, ex2);
            if (max_pair(ex_lo, xmin) < min_pair(ex_hi, xmax)) {
              invalid = true;
              break;
            }
          }
        }
      }

      if (!invalid) {
        max_area = area;
      }
    }
  }
  return max_area;
}

void solve_day9() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day9_part1();
  print_and_send_solution(9, 1, part1_ans, millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day9_part2();
  print_and_send_solution(9, 2, part2_ans, millis() - start_time);
}
