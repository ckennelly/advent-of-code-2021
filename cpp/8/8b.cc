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

  const std::map<int, std::vector<std::pair<int, std::string>>> patterns = {
      {2, {{1, "cf"}}},
      {3, {{7, "acf"}}},
      {4, {{4, "bcdf"}}},
      {5, {{2, "acdeg"}, {3, "acdfg"}, {5, "abdfg"}}},
      {6, {{0, "abcefg"}, {6, "abdefg"}, {9, "abcdfg"}}},
      {7, {{8, "abcdefg"}}},
  };

  const absl::flat_hash_map<std::string, int> to_num = {
      {"cf", 1},    {"acf", 7},    {"bcdf", 4},   {"acdeg", 2},  {"acdfg", 3},
      {"abdfg", 5}, {"abcefg", 0}, {"abdefg", 6}, {"abcdfg", 9}, {"abcdefg", 8},
  };

  int sum = 0;
  for (const auto& line : lines) {
    std::vector<std::string> s = absl::StrSplit(line, '|');
    assert(s.size() == 2);
    absl::flat_hash_set<std::string> unique =
        absl::StrSplit(s[0], ' ', absl::SkipEmpty());
    assert(unique.size() == 10);
    std::vector<std::string> output =
        absl::StrSplit(s[1], ' ', absl::SkipEmpty());
    assert(output.size() == 4);

    const std::string remap = "abcdefg";
    std::string belief = "abcdefg";

    do {
      bool valid = true;

      absl::flat_hash_map<char, char> remapping;
      for (int i = 0; i < remap.size(); i++) {
        remapping[remap[i]] = belief[i];
      }

      for (std::string word : unique) {
        for (auto& c : word) {
          c = remapping[c];
        }

        std::sort(word.begin(), word.end());

        bool found = false;
        for (const auto& kv : patterns.at(word.size())) {
          if (kv.second == word) {
            found = true;
            break;
          }
        }

        if (!found) {
          valid = false;
          break;
        }
      }

      if (valid) {
        break;
      }
    } while (std::next_permutation(belief.begin(), belief.end()));

    absl::flat_hash_map<char, char> remapping;
    for (int i = 0; i < remap.size(); i++) {
      remapping[remap[i]] = belief[i];
    }

    // Run through output
    int value = 0;
    for (std::string word : output) {
      for (auto& c : word) {
        c = remapping[c];
      }

      std::sort(word.begin(), word.end());

      const auto it = to_num.find(word);
      assert(it != to_num.end());
      value *= 10;
      value += it->second;
    }

    sum += value;
  }

  printf("%d\n", sum);
}
