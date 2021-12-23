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

#include "22/common.h"
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

using namespace aoc2021_22;

using XYZ = std::tuple<int, int, int>;

int main(int argc, char** argv) {
  absl::flat_hash_map<XYZ, bool> state;
  std::string input = readAll();

  for (absl::string_view s : absl::StrSplit(input, '\n', absl::SkipEmpty())) {
    bool new_state = absl::StartsWith(s, "on");
    std::string line;
    if (new_state) {
      line = std::string(s.substr(3));
    } else {
      line = std::string(s.substr(4));
    }
    int min_x, max_x, min_y, max_y, min_z, max_z;
    sscanf(line.c_str(), "x=%d..%d,y=%d..%d,z=%d..%d", &min_x, &max_x, &min_y,
           &max_y, &min_z, &max_z);

    min_x = std::max(-50, min_x);
    max_x = std::min(50, max_x);
    min_y = std::max(-50, min_y);
    max_y = std::min(50, max_y);
    min_z = std::max(-50, min_z);
    max_z = std::min(50, max_z);

    for (int x = min_x; x <= max_x; x++) {
      for (int y = min_y; y <= max_y; y++) {
        for (int z = min_z; z <= max_z; z++) {
          state[{x, y, z}] = new_state;
        }
      }
    }
  }

  long count = 0;
  for (auto& kv : state) {
    count += static_cast<long>(kv.second);
  }

  absl::PrintF("%d\n", count);
}
