#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
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

int main(int argc, char** argv) {
  std::string input = readAll();
  absl::flat_hash_map<int, uint64_t> fish;
  for (auto sv : absl::StrSplit(input, ',', absl::SkipEmpty())) {
    int a;
    if (absl::SimpleAtoi(sv, &a)) {
      fish[a]++;
    } else {
      assert(false);
    }
  }

  for (int i = 0; i < 256; i++) {
    absl::flat_hash_map<int, uint64_t> next;

    for (const auto& kv : fish) {
      if (kv.first >= 1) {
        next[kv.first - 1] += kv.second;
      } else {
        next[6] += kv.second;
        next[8] += kv.second;
      }
    }

    fish = std::move(next);
  }

  uint64_t count = 0;
  for (const auto& kv : fish) {
    count += kv.second;
  }
  printf("%llu\n", count);
}
