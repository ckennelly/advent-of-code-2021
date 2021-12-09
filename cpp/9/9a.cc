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

  int risk = 0;
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

      risk += 1 + (lines[y][x] - '0');
    }
  }

  printf("%d\n", risk);
}
