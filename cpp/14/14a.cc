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

int main(int argc, char** argv) {
  std::string input = readAll();
  std::vector<std::string> lines =
      absl::StrSplit(input, '\n', absl::SkipEmpty());
  assert(lines.size() >= 2);

  std::string seed = lines[0];
  absl::flat_hash_map<std::pair<char, char>, char> conversions;
  for (int i = 1; i < lines.size(); i++) {
    char a, b, c;
    sscanf(lines[i].c_str(), "%c%c -> %c", &a, &b, &c);
    conversions[{a, b}] = c;
  }

  for (int i = 0; i < 10; i++) {
    std::string new_seed;
    for (int j = 0; j < seed.size() - 1; j++) {
      char a = seed[j];
      char b = seed[j + 1];
      char insert = conversions.at({a, b});
      if (j == 0) {
        new_seed.append(1, a);
      }
      new_seed.append(1, insert);
      new_seed.append(1, b);
    }

    seed = std::move(new_seed);
  }

  absl::flat_hash_map<char, int> counts;
  for (auto c : seed) {
    counts[c]++;
  }

  int min_count = INT_MAX, max_count = INT_MIN;
  for (auto& kv : counts) {
    min_count = std::min(kv.second, min_count);
    max_count = std::max(kv.second, max_count);
  }

  printf("%d\n", max_count - min_count);
}
