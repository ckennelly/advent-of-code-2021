#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

std::string readAll() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  return buffer.str();
}

int main(int argc, char** argv) {
  std::string input = readAll();
  std::vector<std::string> lines =
      absl::StrSplit(input, '\n', absl::SkipEmpty());

  const int real_x = lines[0].size();
  const int real_y = lines.size();

  const int tx = lines[0].size() * 5;
  const int ty = lines.size() * 5;

  absl::flat_hash_map<std::pair<int, int>, int> costs;
  std::multimap<int, std::pair<int, int>> to_visit;
  to_visit.emplace(0, std::make_pair(0, 0));

  while (!to_visit.empty()) {
    auto it = to_visit.begin();
    int cost = it->first;
    int x = it->second.first;
    int y = it->second.second;

    to_visit.erase(it);

    if (!costs.emplace(std::make_pair(x, y), cost).second) {
      continue;
    }

    const int dx[][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

    for (auto dxy : dx) {
      if (dxy[0] + x < 0 || dxy[0] + x >= tx) {
        continue;
      }
      if (dxy[1] + y < 0 || dxy[1] + y >= ty) {
        continue;
      }

      const int new_x = dxy[0] + x;
      const int new_y = dxy[1] + y;

      int base_cost = lines[new_y % real_y][new_x % real_x] - '0';
      int added_cost = (new_y / real_y) + (new_x / real_x);
      int new_cost = base_cost + added_cost;
      while (new_cost > 9) {
        new_cost -= 9;
      }
      assert(new_cost >= 1);
      assert(new_cost <= 9);

      to_visit.emplace(cost + new_cost, std::make_pair(dxy[0] + x, dxy[1] + y));
    }
  }

  printf("%d\n", costs.at({tx - 1, ty - 1}));
}
