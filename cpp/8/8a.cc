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

  std::map<int, std::vector<std::pair<int, std::string>>> patterns = {
      {2, {{1, "cf"}}},
      {3, {{7, "acf"}}},
      {4, {{4, "bcdf"}}},
      {5, {{2, "acdeg"}, {3, "acdfg"}, {5, "abdfg"}}},
      {6, {{0, "abcefg"}, {6, "abdefg"}, {9, "abcdfg"}}},
      {7, {{8, "abcdefg"}}},
  };

  int count = 0;
  for (const auto& line : lines) {
    std::vector<std::string> s = absl::StrSplit(line, '|');
    assert(s.size() == 2);
    absl::flat_hash_set<std::string> unique =
        absl::StrSplit(s[0], ' ', absl::SkipEmpty());
    assert(unique.size() == 10);
    std::vector<std::string> output =
        absl::StrSplit(s[1], ' ', absl::SkipEmpty());
    assert(output.size() == 4);

    for (const auto& s : output) {
      if (patterns[s.size()].size() == 1) {
        count++;
      }
    }
  }

  printf("%d\n", count);
}
