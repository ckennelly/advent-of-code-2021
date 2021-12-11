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

  const int tx = lines[0].size();
  const int ty = lines.size();

  int total_flashes = 0;
  for (int round = 0; round < 100; round++) {
    for (int x = 0; x < tx; x++) {
      for (int y = 0; y < ty; y++) {
        lines[y][x]++;
      }
    }

    absl::flat_hash_set<std::pair<int, int>> to_visit;
    absl::flat_hash_set<std::pair<int, int>> flashes;
    for (int x = 0; x < tx; x++) {
      for (int y = 0; y < ty; y++) {
        if (lines[y][x] <= '9') {
          continue;
        }

        to_visit.emplace(x, y);
      }
    }

    while (!to_visit.empty()) {
      auto xy = *to_visit.begin();
      to_visit.erase(to_visit.begin());
      const int x = xy.first;
      const int y = xy.second;

      assert(lines[y][x] >= '9');
      if (!flashes.insert(std::pair(x, y)).second) {
        // Already flashed
        continue;
      }

      for (int dx = -1; dx <= 1; dx++) {
        if (x + dx < 0 || x + dx >= tx) {
          continue;
        }

        for (int dy = -1; dy <= 1; dy++) {
          if (dx == 0 && dy == 0) {
            continue;
          }

          if (y + dy < 0 || y + dy >= ty) {
            continue;
          }

          lines[y + dy][x + dx]++;

          if (lines[y + dy][x + dx] <= '9') {
            continue;
          }

          to_visit.emplace(x + dx, y + dy);
        }
      }
    }

    for (auto& xy : flashes) {
      // Reset
      lines[xy.second][xy.first] = '0';
    }

    total_flashes += flashes.size();
  }

  printf("%d\n", total_flashes);
}
