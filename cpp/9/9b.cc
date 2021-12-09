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

std::vector<int> SplitToInts(absl::string_view sv, absl::string_view delim) {
  std::vector<int> r;
  for (absl::string_view chunk :
       absl::StrSplit(sv, delim, absl::SkipWhitespace())) {
    int a;
    if (absl::SimpleAtoi(chunk, &a)) {
      r.push_back(a);
    } else {
      assert(false);
    }
  }
  return r;
}

int main(int argc, char** argv) {
  std::string input = readAll();
  std::vector<std::string> lines =
      absl::StrSplit(input, '\n', absl::SkipEmpty());
  assert(!lines.empty());

  const int tx = lines[0].size();
  const int ty = lines.size();

  std::vector<std::pair<int, int>> seeds;

  for (int y = 0; y < ty; y++) {
    for (int x = 0; x < tx; x++) {
      bool low = true;

      for (int dx = -1; dx <= 1; dx++) {
        if (x + dx < 0 || x + dx >= tx) {
          continue;
        }

        for (int dy = -1; dy <= 1; dy++) {
          if (y + dy < 0 || y + dy >= ty) {
            continue;
          }

          if (dx == 0 && dy == 0) {
            continue;
          }

          if (dx != 0 && dy != 0) {
            continue;
          }

          if (lines[y][x] >= lines[y + dy][x + dx]) {
            low = false;
            break;
          }
        }

        if (!low) {
          break;
        }
      }

      if (!low) {
        continue;
      }

      seeds.emplace_back(x, y);
    }
  }

  std::vector<int> basins;

  for (const auto& seed : seeds) {
    absl::flat_hash_set<std::pair<int, int>> visited;
    absl::flat_hash_set<std::pair<int, int>> in_basin;
    absl::flat_hash_set<std::pair<int, int>> to_visit;
    to_visit.insert(seed);
    in_basin.insert(seed);

    while (!to_visit.empty()) {
      const int x = to_visit.begin()->first;
      const int y = to_visit.begin()->second;
      to_visit.erase(to_visit.begin());

      bool low = true;

      for (int dx = -1; dx <= 1; dx++) {
        if (x + dx < 0 || x + dx >= tx) {
          continue;
        }

        for (int dy = -1; dy <= 1; dy++) {
          if (y + dy < 0 || y + dy >= ty) {
            continue;
          }

          if (dx == 0 && dy == 0) {
            continue;
          }

          if (dx != 0 && dy != 0) {
            continue;
          }

          if (in_basin.contains(std::pair<int, int>(x + dx, y + dy))) {
            continue;
          }

          if (lines[y][x] > lines[y + dy][x + dx]) {
            low = false;
            break;
          }
        }

        if (!low) {
          break;
        }
      }

      if (low && lines[y][x] != '9') {
        in_basin.emplace(x, y);

        for (int dx = -1; dx <= 1; dx++) {
          if (x + dx < 0 || x + dx >= tx) {
            continue;
          }

          for (int dy = -1; dy <= 1; dy++) {
            if (y + dy < 0 || y + dy >= ty) {
              continue;
            }

            if (dx == 0 && dy == 0) {
              continue;
            }

            if (dx != 0 && dy != 0) {
              continue;
            }

            if (in_basin.contains(std::pair<int, int>(x + dx, y + dy))) {
              continue;
            }

            to_visit.emplace(x + dx, y + dy);
          }
        }
      }
    }

    basins.push_back(in_basin.size());
  }

  std::sort(basins.begin(), basins.end());

  int product = 1;
  for (int i = basins.size() - 1, j = 0; i >= 0 && j < 3; --i, ++j) {
    product *= basins[i];
  }

  printf("%d\n", product);
}
