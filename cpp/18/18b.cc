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

#include "18/common.h"
#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"
#include "absl/types/variant.h"

std::string readAll() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  return buffer.str();
}

using namespace aoc2021_18;

int main(int argc, char** argv) {
  std::vector<std::string> lines =
      absl::StrSplit(readAll(), '\n', absl::SkipEmpty());
  int max_magnitude = INT_MIN;
  for (int i = 0; i < lines.size(); i++) {
    for (int j = 0; j < lines.size(); j++) {
      if (i == j) {
        continue;
      }

      std::string add = absl::StrCat("[", lines[i], ",", lines[j], "]");
      reduce(add);

      max_magnitude = std::max(max_magnitude, magnitude(add));
    }
  }

  absl::PrintF("%d\n", max_magnitude);
}
