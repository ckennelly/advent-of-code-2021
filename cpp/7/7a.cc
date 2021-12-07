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
#include "absl/strings/numbers.h"
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
  std::vector<int> pos = SplitToInts(input, ",");

  int min_pos = INT_MAX, max_pos = INT_MIN;
  for (int p : pos) {
    min_pos = std::min(p, min_pos);
    max_pos = std::max(p, max_pos);
  }

  int min_fuel = INT_MAX;
  for (int i = min_pos; i <= max_pos; i++) {
    int fuel = 0;
    for (int p : pos) {
      fuel += std::abs(i - p);
    }

    min_fuel = std::min(min_fuel, fuel);
  }

  printf("%d\n", min_fuel);
}
