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

#include "20/common.h"
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

using namespace aoc2021_20;

int main(int argc, char** argv) {
  std::string input = readAll();
  std::vector<std::string> chunks =
      absl::StrSplit(input, "\n\n", absl::SkipEmpty());
  assert(chunks.size() == 2);

  const std::string enhancement = absl::StrReplaceAll(chunks[0], {{"\n", ""}});
  assert(enhancement.size() == 512);
  std::vector<std::string> raw =
      absl::StrSplit(chunks[1], "\n", absl::SkipEmpty());
  int tx = raw[0].size();
  int ty = raw.size();

  int minx = 0, miny = 0;

  absl::flat_hash_map<std::pair<int, int>, char> state;
  for (int y = 0; y < ty; y++) {
    for (int x = 0; x < tx; x++) {
      state[{x, y}] = raw[y][x];
    }
  }

  const int delta = 3;
  tx += delta;
  ty += delta;
  minx -= delta;
  miny -= delta;

  int default_bit = 0;
  int count;
  for (int i = 0; i < 50; i++) {
    absl::flat_hash_map<std::pair<int, int>, char> out;
    count = 0;
    for (int y = minx; y < ty; y++) {
      for (int x = miny; x < tx; x++) {
        unsigned index = 0;

        for (int dy = -1; dy <= 1; dy++) {
          for (int dx = -1; dx <= 1; dx++) {
            bool bit;
            if (state.contains({x + dx, y + dy})) {
              bit = state[{x + dx, y + dy}] == '#' ? 1 : 0;
            } else {
              bit = default_bit;
            }
            index <<= 1;
            index += bit;
          }
        }

        assert(index < 512);
        assert(index < enhancement.size());
        out[{x, y}] = enhancement[index];
        if (out[{x, y}] == '#') {
          count++;
        }
      }
    }

    state = out;

    minx -= delta;
    miny -= delta;
    tx += delta;
    ty += delta;

    default_bit = enhancement[default_bit * 511] == '#' ? 1 : 0;
  }

  printf("%d\n", count);
}
