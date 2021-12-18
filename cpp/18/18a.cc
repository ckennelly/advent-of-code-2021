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
  std::string input = readAll();

  std::string temp;

  bool first = true;
  for (absl::string_view line :
       absl::StrSplit(input, '\n', absl::SkipEmpty())) {
    if (first) {
      temp = std::string(line);
      first = false;
    } else {
      temp = absl::StrCat("[", temp, ",", line, "]");
    }

    reduce(temp);
  }

  absl::PrintF("%d\n", magnitude(temp));
}
