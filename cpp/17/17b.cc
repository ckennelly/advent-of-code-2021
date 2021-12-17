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

  // target area: x=277..318, y=-92..-53
  int x_min, x_max, y_min, y_max;
  sscanf(input.c_str(), "target area: x=%d..%d, y=%d..%d", &x_min, &x_max,
         &y_min, &y_max);

  absl::flat_hash_set<std::pair<int, int>> inits;
  for (int x = 0; x < 2 * x_max; x++) {
    for (int y = y_min; y < 3 * std::abs(y_min); y++) {
      int pos_x = 0, pos_y = 0;
      int dx = x, dy = y;

      int max_y = INT_MIN;

      bool hit = false;
      for (int t = 0; t < 300; t++) {
        pos_x += dx;
        pos_y += dy;

        max_y = std::max(max_y, pos_y);

        if (pos_x >= x_min && pos_x <= x_max && pos_y >= y_min &&
            pos_y <= y_max) {
          hit = true;
          break;
        }

        if (dx > 0) {
          dx--;
        }
        dy--;
      }

      if (hit) {
        inits.emplace(x, y);
      }
    }
  }

  printf("%zu\n", inits.size());
}
