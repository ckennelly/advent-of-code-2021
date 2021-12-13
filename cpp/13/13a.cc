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
  char dir;
  int xy;
  sscanf(str_folds[0].c_str(), "fold along %c=%d", &dir, &xy);

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

  printf("%zu\n", new_points.size());
}
