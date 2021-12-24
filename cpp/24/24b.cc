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

#include "24/common.h"
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

using namespace aoc2021_24;

int main(int argc, char** argv) {
  const std::string input = readAll();

  const auto program = ParseProgram(input);
  const auto summary = ParseSummary(input);
  const auto relationships = FindRelationships(summary);

  char digits[14];
  memset(digits, 0, sizeof(digits));

  auto convertDigits = [&]() {
    long r = 0;
    for (int i = 0; i < 14; i++) {
      r *= 10;
      r += long(digits[i]);
    }
    return r;
  };

  int found_count = 0;
  for (int i = 0; i < 14; i++) {
    auto it = relationships.find(i);
    if (it == relationships.end()) {
      // We'll revisit when doing the matched pair.
      continue;
    }

    const int a0 = it->first;
    const int a1 = std::get<0>(it->second);
    assert(a0 != a1);
    const long r = std::get<1>(it->second);
    assert(digits[a0] == 0);
    assert(digits[a1] == 0);

    const int amin = std::min(a0, a1);
    const int amax = std::max(a0, a1);
    bool found = false;
    int jmin, jmax;
    for (jmin = 1; jmin <= 9; jmin++) {
      jmax = jmin - r;
      if (jmax < 1 || jmax > 9) {
        continue;
      }

      found = true;
      break;
    }

    assert(found);
    digits[amin] = jmin;
    digits[amax] = jmax;
    found_count += 2;
  }
  assert(found_count == 14);

  // Verify
  assert(RunProgram(program, absl::MakeSpan(digits)) == 0);
  absl::PrintF("%d\n", convertDigits());
}
