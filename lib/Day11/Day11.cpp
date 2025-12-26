#include <Arduino.h>
#include <Day11.h>
#include <Day11Data.h>
#include <SharedMemory.h>

#include <etl/deque.h>
#include <etl/multimap.h>
#include <etl/string.h>

using ServerNode = etl::string<3>;
using ServerMap = etl::multimap<ServerNode, ServerNode, 1650>;

struct PathCount {
  ServerNode node;
  bool has_visited_dac;
  bool has_visited_fft;
  int64_t times_visited;

  bool operator==(const PathCount &other) const {
    if (node == other.node && has_visited_dac == other.has_visited_dac &&
        has_visited_fft == other.has_visited_fft) {
      return true;
    }
    return false;
  }

  bool operator<(const PathCount &other) const {
    if (node != other.node) {
      return node < other.node;
    } else if (has_visited_dac != other.has_visited_dac) {
      return has_visited_dac < other.has_visited_dac;
    }
    return has_visited_fft < other.has_visited_fft;
  }
};

using BfsQueue = etl::deque<PathCount, 600>;

struct Data {
  ServerMap server_map;
  BfsQueue current_queue;
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
    data.current_queue.push_back({it->second, false, false, 1});
  }

  while (!data.current_queue.empty()) {
    PathCount cur_node = data.current_queue.front();
    data.current_queue.pop_front();
    if (cur_node.node == "out") {
      valid_paths += cur_node.times_visited;
      continue;
    }

    auto neighbors = data.server_map.equal_range(cur_node.node);
    for (auto it = neighbors.first; it != neighbors.second; ++it) {
      PathCount next_node = {it->second, false, false, cur_node.times_visited};

      bool found = false;
      for (auto &existing : data.current_queue) {
        if (existing == next_node) {
          existing.times_visited += next_node.times_visited;
          found = true;
          break;
        }
      }

      if (!found) {
        data.current_queue.push_back(next_node);
      }
    }
  }

  return valid_paths;
}

long long solve_day11_part2() {
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

  auto range = data.server_map.equal_range("svr");
  for (auto it = range.first; it != range.second; ++it) {
    data.current_queue.push_back({it->second, false, false, 1});
  }

  while (!data.current_queue.empty()) {
    PathCount cur_node = data.current_queue.front();
    data.current_queue.pop_front();
    if (cur_node.node == "out" && cur_node.has_visited_dac &&
        cur_node.has_visited_fft) {
      valid_paths += cur_node.times_visited;
      continue;
    }

    auto neighbors = data.server_map.equal_range(cur_node.node);
    for (auto it = neighbors.first; it != neighbors.second; ++it) {
      bool visited_dac = (it->second == "dac" || cur_node.has_visited_dac);
      bool visited_fft = (it->second == "fft" || cur_node.has_visited_fft);
      PathCount next_node = {it->second, visited_dac, visited_fft,
                             cur_node.times_visited};

      bool found = false;
      for (auto &existing : data.current_queue) {
        if (existing == next_node) {
          existing.times_visited += next_node.times_visited;
          found = true;
          break;
        }
      }

      if (!found) {
        data.current_queue.push_back(next_node);
      }
    }
  }

  return valid_paths;
}

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
