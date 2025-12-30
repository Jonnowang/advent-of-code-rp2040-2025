#include <Arduino.h>
#include <Day8.h>
#include <Day8Data.h>
#include <SharedMemory.h>
#include <Utils.h>

#include <etl/algorithm.h>
#include <etl/map.h>
#include <etl/string.h>
#include <etl/to_arithmetic.h>
#include <etl/vector.h>

#define MAX_POINTS 1000

struct JunctionBox {
  int x, y, z;
};

struct Edge {
  int u, v;
  long long distSq; // Squared Euclidean distance

  bool operator<(const Edge &other) const { return distSq < other.distSq; }
};

long long solve_day8_part1() {
  struct Data {
    etl::vector<JunctionBox, 1000> boxes;
    etl::vector<Edge, 1000> top_edges;
    etl::vector<int, 1000> parent;
    etl::vector<int, 1000> circuit_size;
  };
  StaticMemoryBuffer<Data> d_wrapper;
  auto &d = *d_wrapper;

  etl::string<10> num_str;
  etl::vector<int, 3> coords;

  for (const char *p = day8_data; *p; p++) {
    if (*p >= '0' && *p <= '9') {
      num_str += *p;
    } else {
      coords.push_back(etl::to_arithmetic<int>(num_str));
      num_str.clear();
      if (*p == '\n' || coords.full()) {
        d.boxes.push_back({coords[0], coords[1], coords[2]});
        coords.clear();
      }
    }
  }

  auto &max_heap = d.top_edges;

  for (int i = 0; i < d.boxes.size(); i++) {
    for (int j = i + 1; j < d.boxes.size(); j++) {
      long long dx = d.boxes[i].x - d.boxes[j].x;
      long long dy = d.boxes[i].y - d.boxes[j].y;
      long long dz = d.boxes[i].z - d.boxes[j].z;
      long long dist = dx * dx + dy * dy + dz * dz;

      // Quick-ish way to store the 1000 shortest distances
      // because not enough RAM to store all of them
      if (max_heap.size() < d.boxes.size()) {
        max_heap.push_back({i, j, dist});
        etl::push_heap(max_heap.begin(), max_heap.end());
      } else if (dist < max_heap.front().distSq) {
        etl::pop_heap(max_heap.begin(), max_heap.end());
        max_heap.back() = {i, j, dist};
        etl::push_heap(max_heap.begin(), max_heap.end());
      }
    }
  }

  for (int i = 0; i < d.boxes.size(); i++) {
    d.parent[i] = i;
    d.circuit_size[i] = 1;
  }

  auto find = [&](int i) {
    int root = i;
    while (root != d.parent[root])
      root = d.parent[root];
    return root;
  };

  auto unite = [&](int i, int j) {
    i = find(i);
    j = find(j);
    // If two junctions don't belong to the same circuit, connect them
    if (i != j) {
      d.parent[j] = i;
      d.circuit_size[i] += d.circuit_size[j];
    }
  };

  // Connect the 1000 shortest junctions
  for (const auto &edge : max_heap)
    unite(edge.u, edge.v);

  long long top[3] = {0};
  for (int i = 0; i < d.boxes.size(); i++) {
    if (d.parent[i] == i) { // If is reference node
      long long s = d.circuit_size[i];
      if (s > top[0]) {
        top[2] = top[1];
        top[1] = top[0];
        top[0] = s;
      } else if (s > top[1]) {
        top[2] = top[1];
        top[1] = s;
      } else if (s > top[2]) {
        top[2] = s;
      }
    }
  }

  return top[0] * top[1] * top[2];
}

long long solve_day8_part2() {
  struct Data {
    etl::vector<JunctionBox, 1000> boxes;
    etl::vector<Edge, 4000> top_edges;
    etl::vector<int, 1000> parent;
    etl::vector<int, 1000> circuit_size;
  };
  StaticMemoryBuffer<Data> d_wrapper;
  auto &d = *d_wrapper;

  etl::string<10> num_str;
  etl::vector<int, 3> coords;
  int already_seen_size = 0;

  for (const char *p = day8_data; *p; p++) {
    if (*p >= '0' && *p <= '9') {
      num_str += *p;
    } else {
      coords.push_back(etl::to_arithmetic<int>(num_str));
      num_str.clear();
      if (*p == '\n' || coords.full()) {
        d.boxes.push_back({coords[0], coords[1], coords[2]});
        coords.clear();
      }
    }
  }

  auto &max_heap = d.top_edges;

  auto construct_heap = [&](int min_dist) {
    max_heap = {};

    for (int i = 0; i < d.boxes.size(); i++) {
      for (int j = i + 1; j < d.boxes.size(); j++) {
        long long dx = d.boxes[i].x - d.boxes[j].x;
        long long dy = d.boxes[i].y - d.boxes[j].y;
        long long dz = d.boxes[i].z - d.boxes[j].z;
        long long dist = dx * dx + dy * dy + dz * dz;

        if (dist >= min_dist) {
          if (!max_heap.full()) {
            max_heap.push_back({i, j, dist});
            etl::push_heap(max_heap.begin(), max_heap.end());
          } else if (dist < max_heap.front().distSq) {
            etl::pop_heap(max_heap.begin(), max_heap.end());
            max_heap.back() = {i, j, dist};
            etl::push_heap(max_heap.begin(), max_heap.end());
          }
        }
      }
    }

    etl::sort_heap(max_heap.begin(), max_heap.end());
    already_seen_size = max_heap.back().distSq;
  };

  for (int i = 0; i < d.boxes.size(); i++) {
    d.parent[i] = i;
    d.circuit_size[i] = 1;
  }

  auto find = [&](int i) {
    int root = i;
    while (root != d.parent[root])
      root = d.parent[root];
    return root;
  };

  JunctionBox final_boxes[2];
  bool found = false;

  auto unite = [&](int x, int y) -> bool {
    int i = find(x);
    int j = find(y);
    // If two junctions don't belong to the same circuit, connect them
    if (i != j) {
      d.parent[j] = i;
      if (d.circuit_size[i] + d.circuit_size[j] == 1000) {
        final_boxes[0] = d.boxes[x];
        final_boxes[1] = d.boxes[y];
        return true;
      }
      d.circuit_size[i] += d.circuit_size[j];
    }
    return false;
  };

  while (1) {
    // Hacky way to save RAM by just reconstructing the heap if we've exhausted
    // it and not conncted all of the junctions
    construct_heap(already_seen_size);
    for (const auto &edge : max_heap) {
      if (unite(edge.u, edge.v)) {
        found = true;
        break;
      }
    }
    if (found)
      break;
  }

  return (long long)final_boxes[0].x * final_boxes[1].x;
}

void solve_day8() {
  unsigned long start_time = millis();
  long long part1_ans = solve_day8_part1();
  print_and_send_solution(8, 1, part1_ans, millis() - start_time);

  start_time = millis();
  long long part2_ans = solve_day8_part2();
  print_and_send_solution(8, 2, part2_ans, millis() - start_time);
}
