#include <Arduino.h>
#include <Day10.h>
#include <Day10Data.h>
#include <SharedMemory.h>
#include <new>

#include <etl/set.h>
#include <etl/vector.h>

long long solve_day10_part1() {
  using State = etl::vector<bool, 10>;
  using StateArray = etl::vector<State, 1024>;
  using StateSet = etl::set<State, 1024>;
  using SwitchOperations = etl::vector<uint8_t, 10>;
  using SwitchArray = etl::vector<SwitchOperations, 20>;

  struct Data {
    StateArray current_states;
    StateArray next_states;
    StateSet visited_states;
  };

  StaticMemoryBuffer<Data> mem_ptr;
  auto &d = *mem_ptr;

  SwitchOperations switch_buf;
  SwitchArray switches;
  State state_buf;

  long long shortest = 0;
  bool parsing_switch = false;

  auto solve_case = [&]() {
    bool found = false;
    while (!found && !d.current_states.empty()) {
      for (const auto &cur_state : d.current_states) {
        for (const auto &cur_switch : switches) {
          State new_state = cur_state;
          for (auto flip : cur_switch)
            new_state[flip] = !new_state[flip];

          if (d.visited_states.find(new_state) ==
              d.visited_states.end()) {
            d.next_states.push_back(new_state);
            d.visited_states.insert(new_state);

            bool all_zero = true;
            for (auto state_bit : new_state) {
              if (state_bit) {
                all_zero = false;
                break;
              }
            }
            if (all_zero) {
              found = true;
              break;
            }
          }
        }
        if (found)
          break;
      }
      shortest++;
      d.current_states = d.next_states;
      d.next_states.clear();
    }
    d.current_states.clear();
    d.next_states.clear();
    d.visited_states.clear();
    switches.clear();
  };

  for (const char *p = day10_data; *p; p++) {
    if (*p == '.') {
      state_buf.push_back(false);
    } else if (*p == '#') {
      state_buf.push_back(true);
    } else if (*p == ']') {
      d.current_states.push_back(state_buf);
      d.visited_states.insert(state_buf);
      state_buf.clear();
    } else if (*p == '\n') {
      solve_case();
    } else {
      if (*p == '(') {
        parsing_switch = true;
      } else if (*p >= '0' && *p <= '9' && parsing_switch) {
        switch_buf.push_back(*p - '0');
      } else if (*p == ')') {
        switches.push_back(switch_buf);
        switch_buf.clear();
        parsing_switch = false;
      }
    }
  }

  return shortest;
}

long long solve_day10_part2() { return 0LL; }

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
