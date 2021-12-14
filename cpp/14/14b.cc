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

  absl::flat_hash_map<std::pair<char, char>, long> pairs;
  for (int i = 0; i < seed.size() - 1; i++) {
    pairs[{seed[i], seed[i + 1]}]++;
  }

  for (int i = 0; i < 40; i++) {
    absl::flat_hash_map<std::pair<char, char>, long> new_pairs;
    new_pairs.reserve(pairs.size());

    for (auto kv : pairs) {
      char a = kv.first.first;
      char b = kv.first.second;
      long count = kv.second;

      char insert = conversions.at({a, b});

      new_pairs[{a, insert}] += count;
      new_pairs[{insert, b}] += count;
    }

    pairs = std::move(new_pairs);
  }

  absl::flat_hash_map<char, long> counts;
  for (auto kv : pairs) {
    counts[kv.first.first] += kv.second;
    counts[kv.first.second] += kv.second;
  }
  counts[seed[0]]++;
  counts[seed[seed.size() - 1]]++;

  long min_count = std::numeric_limits<long>::max(),
       max_count = std::numeric_limits<long>::min();
  for (auto& kv : counts) {
    min_count = std::min(kv.second, min_count);
    max_count = std::max(kv.second, max_count);
  }

  printf("%ld\n", (max_count - min_count) / 2);
}
