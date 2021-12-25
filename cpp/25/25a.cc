#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "25/common.h"
#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"
#include "absl/types/variant.h"

std::string readAll() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  return buffer.str();
}

using namespace aoc2021_25;

int main(int argc, char** argv) {
  std::string input = readAll();
  std::vector<std::string> lines =
      absl::StrSplit(input, '\n', absl::SkipEmpty());
  assert(!lines.empty());
  const int tx = lines[0].size();
  const int ty = lines.size();

  auto wrapXY = [&](int x, int y) {
    if (x >= tx) {
      x = 0;
    }
    if (y >= ty) {
      y = 0;
    }

    return std::make_pair(x, y);
  };

  for (int step = 1;; step++) {
    std::vector<std::string> next(ty, std::string(tx, '.'));

    // Do eastbound
    for (int y = 0; y < ty; y++) {
      for (int x = 0; x < tx; x++) {
        if (lines[y][x] != '>') {
          continue;
        }

        const auto& [next_x, next_y] = wrapXY(x + 1, y);
        const char nextChar = lines[next_y][next_x];
        if (nextChar != '.') {
          // Can't move
          next[y][x] = '>';
          continue;
        }

        next[next_y][next_x] = '>';
      }
    }

    // Do southbound
    for (int y = 0; y < ty; y++) {
      for (int x = 0; x < tx; x++) {
        if (lines[y][x] != 'v') {
          continue;
        }

        const auto& [next_x, next_y] = wrapXY(x, y + 1);
        char nextChar = lines[next_y][next_x];
        if (nextChar != '.' && nextChar != '>') {
          // Can't move
          next[y][x] = 'v';
          continue;
        }

        nextChar = next[next_y][next_x];
        if (nextChar == '>') {
          next[y][x] = 'v';
          continue;
        }

        next[next_y][next_x] = 'v';
      }
    }

    if (next == lines) {
      printf("%d\n", step);
      return 0;
    }

    lines = std::move(next);
  }
}
