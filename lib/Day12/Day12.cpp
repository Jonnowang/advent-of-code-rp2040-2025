#include <Arduino.h>
#include <Day12.h>
#include <Day12Data.h>
#include <SharedMemory.h>
#include <Utils.h>

#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

namespace {

using PresentShapes = etl::vector<etl::vector<bool, 3>, 3>;
using Presents = etl::vector<PresentShapes, 6>;

struct Region {
  uint8_t width;
  uint8_t height;
  etl::vector<uint8_t, 6> num_presents;
};

using Regions = etl::vector<Region, 1000>;

struct Data {
  Presents presents;
  Regions regions;
};

} // namespace

long long solve_day12_part1() {
  StaticMemoryBuffer<Data> mem_ptr;
  auto &data = *mem_ptr;

  bool region_mode = false;

  uint8_t present_num;
  etl::string<3> size_buf;
  Region region_buf;

  long long valid_regions = 0;

  for (const char *p = day12_data; *p; p++) {
    if (*p >= '0' && *p <= '9' && !region_mode) {
      present_num = *p - '0';
      if (data.presents.size() <= present_num) {
        data.presents.resize(present_num + 1);
        data.presents[present_num].resize(3);
      }
    } else if (*p == '#' || *p == '.') {
      bool val = (*p == '#');
      for (auto &row : data.presents[present_num]) {
        if (!row.full()) {
          row.push_back(val);
          break;
        }
      }
    } else if (*p >= '0' && *p <= '9' && region_mode) {
      size_buf += *p;
    } else if (*p == 'x' && region_mode) {
      region_buf.width = etl::to_arithmetic<uint8_t>(size_buf);
      size_buf = "";
    } else if (*p == ':' && region_mode) {
      region_buf.height = etl::to_arithmetic<uint8_t>(size_buf);
      size_buf = "";
      p++;
    } else if (*p == ' ' && region_mode) {
      region_buf.num_presents.push_back(etl::to_arithmetic<uint8_t>(size_buf));
      size_buf = "";
    } else if (*p == '\n' && region_mode) {
      region_buf.num_presents.push_back(etl::to_arithmetic<uint8_t>(size_buf));
      data.regions.push_back(region_buf);
      size_buf = "";
      region_buf = {};
    }

    if (data.presents.full() && *p == '\n' && p[-1] == '\n') {
      region_mode = true;
    }
  }

  for (auto &region : data.regions) {
    uint16_t total_area = (uint16_t)region.height * (uint16_t)region.width;
    uint16_t total_present_ct = 0;

    for (auto &ct : region.num_presents) {
      total_present_ct += ct;
    }
    if ((total_area) > (total_present_ct * 8)) {
      valid_regions++;
    }
  }

  return valid_regions;
}

void solve_day12() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day12_part1();
  print_and_send_solution(12, 1, part1_ans, millis() - start_time);
}
