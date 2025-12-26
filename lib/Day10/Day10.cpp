#include <Arduino.h>
#include <Day10.h>
#include <Day10Data.h>
#include <SharedMemory.h>

#include <etl/map.h>
#include <etl/queue.h>
#include <etl/set.h>
#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

using State = etl::vector<bool, 10>;
using JoltageState = etl::vector<int16_t, 10>;

using SwitchOperations = etl::vector<uint8_t, 10>;
using SwitchArray = etl::vector<SwitchOperations, 20>;

using Path = etl::vector<uint8_t, 20>;
using PathList = etl::vector<Path, 20>;

struct SearchNode {
  State state;
  int8_t last_switch_index;
  Path path;
};

struct JoltageNode {
  JoltageState j_state;
  int16_t parent_path_cost;
};

struct LayerNode {
  JoltageState state;
  long long cost;

  bool operator<(const LayerNode &other) const {
    if (state != other.state) {
      return state < other.state;
    }
    return cost < other.cost;
  }
};

using Layer = etl::vector<LayerNode, 400>;

using SearchQueue = etl::queue<SearchNode, 1000>;
using JoltageQueue = etl::queue<JoltageNode, 1000>;

struct Data {
  SearchQueue bfs_queue;
  JoltageQueue joltage_queue;
  Layer current_layer;
  Layer next_layer;
};

bool is_all_zero(const State &state) {
  for (auto bit : state) {
    if (bit) {
      return false;
    }
  }
  return true;
}

bool is_valid_state(const JoltageState &state) {
  for (auto bit : state) {
    if (bit < 0) {
      return false;
    }
  }
  return true;
}

bool is_all_zero_joltage(const JoltageState &state) {
  for (auto bit : state) {
    if (bit != 0) {
      return false;
    }
  }
  return true;
}

void reset_solver_state(Data &data, SwitchArray &switches) {
  while (!data.bfs_queue.empty()) {
    data.bfs_queue.pop();
  }
  data.current_layer.clear();
  data.next_layer.clear();
  switches.clear();
}

State construct_state_from_odd_states(const JoltageState &joltage_data) {
  State output_state = {};
  for (auto &state_count : joltage_data) {
    if (state_count % 2 == 0) {
      output_state.push_back(false);
    } else {
      output_state.push_back(true);
    }
  }
  return output_state;
}

long long find_shortest_path_to_zero(Data &data, const SwitchArray &switches) {
  long long steps = 0;

  if (data.bfs_queue.empty())
    return 0;

  while (!data.bfs_queue.empty()) {
    size_t level_size = data.bfs_queue.size();
    bool found_in_level = false;

    for (size_t k = 0; k < level_size; ++k) {
      SearchNode cur_node = data.bfs_queue.front();
      data.bfs_queue.pop();

      // Try adding switches that have index > last_switch_index
      for (size_t i = cur_node.last_switch_index + 1; i < switches.size();
           ++i) {
        State new_state = cur_node.state;
        for (auto flip : switches[i]) {
          new_state[flip] = !new_state[flip];
        }

        if (is_all_zero(new_state)) {
          return steps + 1;
        }

        data.bfs_queue.push({new_state, (int8_t)i, {}});
      }
    }
    steps++;
  }

  return 0;
}

PathList find_all_paths_to_zero(Data &data, const SwitchArray &switches) {
  PathList valid_paths;

  while (!data.bfs_queue.empty()) {
    SearchNode cur_node = data.bfs_queue.front();
    data.bfs_queue.pop();

    for (size_t i = cur_node.last_switch_index + 1; i < switches.size(); ++i) {
      State new_state = cur_node.state;
      for (auto flip : switches[i]) {
        new_state[flip] = !new_state[flip];
      }

      auto new_path = cur_node.path;
      new_path.push_back((uint8_t)i);

      if (is_all_zero(new_state)) {
        valid_paths.push_back(new_path);
      }

      data.bfs_queue.push({new_state, (int8_t)i, new_path});
    }
  }

  return valid_paths;
}

long long solve_iterative(Data &data, const SwitchArray &switches,
                          JoltageState initial_state) {
  data.current_layer.clear();
  data.next_layer.clear();
  data.current_layer.push_back({initial_state, 0});

  long long min_solution = -1;
  long long multiplier = 1LL;

  while (!data.current_layer.empty()) {
    data.next_layer.clear();
    multiplier <<= 1;

    for (const auto &node : data.current_layer) {
      if (is_all_zero_joltage(node.state)) {
        if (min_solution == -1 || node.cost < min_solution)
          min_solution = node.cost;
        continue;
      }

      // If we already found a solution better than current cost, skip
      if (min_solution != -1 && node.cost >= min_solution)
        continue;

      State bin_state = construct_state_from_odd_states(node.state);
      data.bfs_queue.push({bin_state, -1, {}});
      PathList candidates = find_all_paths_to_zero(data, switches);

      if (is_all_zero(bin_state))
        candidates.push_back({});

      for (const auto &p : candidates) {
        JoltageState next_state = node.state;

        for (auto switch_idx : p) {
          for (auto button : switches[switch_idx]) {
            next_state[button]--;
          }
        }

        if (!is_valid_state(next_state))
          continue;

        for (auto &val : next_state) {
          val /= 2;
        }

        long long new_cost =
            node.cost + (long long)p.size() * (multiplier >> 1);
        data.next_layer.push_back({next_state, new_cost});
      }
    }

    if (!data.next_layer.empty()) {
      etl::sort(data.next_layer.begin(), data.next_layer.end());

      auto new_end = std::unique(data.next_layer.begin(), data.next_layer.end(),
                                 [](const LayerNode &a, const LayerNode &b) {
                                   return a.state == b.state;
                                 });
      data.next_layer.resize(new_end - data.next_layer.begin());
    }

    data.current_layer = data.next_layer;
  }

  return min_solution;
}

long long solve_day10_part1() {
  StaticMemoryBuffer<Data> mem_ptr;
  auto &data = *mem_ptr;

  SwitchOperations switch_buf;
  SwitchArray switches;
  State state_buf;

  long long shortest = 0;
  bool parsing_switch = false;

  for (const char *p = day10_data; *p; p++) {
    if (*p == '.') {
      state_buf.push_back(false);
    } else if (*p == '#') {
      state_buf.push_back(true);
    } else if (*p == ']') {
      // Push initial state to queue
      data.bfs_queue.push({state_buf, -1, {}});
      state_buf.clear();
    } else if (*p == '\n') {
      long long res = find_shortest_path_to_zero(data, switches);
      shortest += res;
      reset_solver_state(data, switches);
    } else if (*p == '(') {
      parsing_switch = true;
    } else if (*p == ')') {
      switches.push_back(switch_buf);
      switch_buf.clear();
      parsing_switch = false;
    } else if (*p >= '0' && *p <= '9' && parsing_switch) {
      switch_buf.push_back(*p - '0');
    }
  }

  return shortest;
}

long long solve_day10_part2() {
  StaticMemoryBuffer<Data> mem_ptr;
  auto &data = *mem_ptr;

  SwitchOperations switch_buf;
  JoltageState state_buf;
  SwitchArray switches;
  etl::string<4> char_buf;

  JoltageState initial_joltage_state = {};

  long long total_shortest = 0;
  bool parsing_switch = false;
  bool joltage_switch = false;

  for (const char *p = day10_test; *p; p++) {
    if (*p == '\n') {
      long long res = solve_iterative(data, switches, initial_joltage_state);
      total_shortest += res;
      reset_solver_state(data, switches);
      initial_joltage_state = {};
    } else if (*p == '(') {
      parsing_switch = true;
    } else if (*p == ')') {
      switches.push_back(switch_buf);
      switch_buf.clear();
      parsing_switch = false;
    } else if (*p == '{') {
      joltage_switch = true;
    } else if (*p == '}') {
      state_buf.push_back(etl::to_arithmetic<int16_t>(char_buf));
      initial_joltage_state = state_buf;
      state_buf.clear();
      char_buf.clear();
      joltage_switch = false;
    } else if (*p >= '0' && *p <= '9' && parsing_switch) {
      switch_buf.push_back(*p - '0');
    } else if (*p >= '0' && *p <= '9' && joltage_switch) {
      char_buf.push_back(*p);
    } else if (*p == ',' && joltage_switch) {
      state_buf.push_back(etl::to_arithmetic<int16_t>(char_buf));
      char_buf.clear();
    }
  }

  return total_shortest;
}

void solve_day10() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day10_part1();
  Serial.print("Day 10 Part 1 Solution: ");
  Serial.print(part1_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day10_part2();
  Serial.print("Day 10 Part 2 Solution: ");
  Serial.print(part2_ans);
  Serial.printf(" --- Time: %lu ms\n", millis() - start_time);
}
