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
  std::vector<std::string> groups =
      absl::StrSplit(input, "\n\n", absl::SkipEmpty());
  assert(groups.size() == 2);
  std::vector<std::string> str_points =
      absl::StrSplit(groups[0], '\n', absl::SkipEmpty());
  std::vector<std::string> str_folds =
      absl::StrSplit(groups[1], '\n', absl::SkipEmpty());

  absl::flat_hash_set<std::pair<int, int>> points;
  for (auto str_point : str_points) {
    int x, y;
    sscanf(str_point.c_str(), "%d,%d", &x, &y);
    points.emplace(x, y);
  }

  assert(str_folds.size() >= 1);
  for (auto str_fold : str_folds) {
    char dir;
    int xy;
    sscanf(str_fold.c_str(), "fold along %c=%d", &dir, &xy);

    assert(dir == 'x' || dir == 'y');

    absl::flat_hash_set<std::pair<int, int>> new_points;
    for (auto point : points) {
      switch (dir) {
        case 'x':
          if (point.first >= xy) {
            point.first = 2 * xy - point.first;
          }
          break;
        case 'y':
          if (point.second >= xy) {
            point.second = 2 * xy - point.second;
          }
          break;
      }
      new_points.emplace(point);
    }
    points = std::move(new_points);
  }

  int max_x = 0, max_y = 0;
  for (auto xy : points) {
    max_x = std::max(xy.first, max_x);
    max_y = std::max(xy.second, max_y);
  }

  for (int y = 0; y <= max_y; y++) {
    for (int x = 0; x <= max_x; x++) {
      printf("%c", points.contains({x, y}) ? '#' : '.');
    }
    printf("\n");
  }
}
