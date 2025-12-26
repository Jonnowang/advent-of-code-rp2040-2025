#include <Arduino.h>
#include <Day11.h>
#include <Day11Data.h>
#include <SharedMemory.h>
#include <new>

#include <etl/multimap.h>
#include <etl/queue.h>
#include <etl/set.h>
#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

using ServerNode = etl::string<3>;
using ServerMap = etl::multimap<ServerNode, ServerNode, 2000>;

struct PathCount {
  ServerNode node;
  int16_t times_visited;

  bool operator<(const PathCount &other) const { return node < other.node; }
};

using BfsQueue = etl::vector<PathCount, 200>;

struct Data {
  ServerMap server_map;
  BfsQueue current_queue;
  BfsQueue next_queue;
};

long long solve_day11_part1() {
  StaticMemoryBuffer<Data> mem_ptr;
  auto &data = *mem_ptr;

  bool child_mode = false;
  long long valid_paths = 0;

  ServerNode parent_buf = "";
  ServerNode name_buf = "";

  for (const char *p = day11_data; *p; p++) {
    if (*p == '\n') {
      data.server_map.insert({parent_buf, name_buf});
      parent_buf.clear();
      name_buf.clear();
      child_mode = false;
    } else if (*p == ':') {
      child_mode = true;
    } else if (*p == ' ') {
      if (!name_buf.empty()) {
        data.server_map.insert({parent_buf, name_buf});
        name_buf.clear();
      }
    } else if (child_mode) {
      name_buf += *p;
    } else {
      parent_buf += *p;
    }
  }

  auto range = data.server_map.equal_range("you");
  for (auto it = range.first; it != range.second; ++it) {
    data.current_queue.push_back({it->second, 1});
  }

  while (!data.current_queue.empty()) {
    data.next_queue.clear();

    for (auto &node : data.current_queue) {
      auto neighbors = data.server_map.equal_range(node.node);
      for (auto it = neighbors.first; it != neighbors.second; ++it) {
        data.next_queue.push_back({it->second, node.times_visited});
      }
    }

    if (!data.next_queue.empty()) {
      etl::sort(data.next_queue.begin(), data.next_queue.end());

      size_t write_idx = 0;
      for (size_t read_idx = 1; read_idx < data.next_queue.size(); ++read_idx) {
        if (data.next_queue[write_idx].node == data.next_queue[read_idx].node) {
          data.next_queue[write_idx].times_visited +=
              data.next_queue[read_idx].times_visited;
        } else {
          write_idx++;
          data.next_queue[write_idx] = data.next_queue[read_idx];
        }
      }
      data.next_queue.resize(write_idx + 1);
    }

    for (auto &nn : data.next_queue) {
      if (nn.node == "out") {
        valid_paths += nn.times_visited;
      }
    }

    data.current_queue = data.next_queue;
  }

  return valid_paths;
}

long long solve_day11_part2() { return 0; }

void solve_day11() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day11_part1();
  Serial.print("Day 11 Part 1 Solution: ");
  Serial.print(part1_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day11_part2();
  Serial.print("Day 11 Part 2 Solution: ");
  Serial.print(part2_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);
}
