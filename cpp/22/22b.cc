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

struct Cube {
  long min_x, max_x;
  long min_y, max_y;
  long min_z, max_z;
  bool on;

  long Volume() const {
    // Given a cube with x=0..0, we touch "0" when turning on the bit, so avoid
    // the fencepost error.
    return (max_x - min_x + 1) * (max_y - min_y + 1) * (max_z - min_z + 1);
  }
};

static absl::optional<std::pair<long, long>> overlap(long min_a, long max_a,
                                                     long min_b, long max_b) {
  if (max_a < min_b || min_a > max_b) {
    return absl::nullopt;
  }

  long values[4] = {min_a, max_a, min_b, max_b};
  std::sort(std::begin(values), std::end(values));
  return std::make_pair(values[1], values[2]);
}

static absl::optional<Cube> overlap(const Cube& a, const Cube& b) {
  auto overlap_x = overlap(a.min_x, a.max_x, b.min_x, b.max_x);
  if (!overlap_x.has_value()) {
    return absl::nullopt;
  }
  auto overlap_y = overlap(a.min_y, a.max_y, b.min_y, b.max_y);
  if (!overlap_y.has_value()) {
    return absl::nullopt;
  }
  auto overlap_z = overlap(a.min_z, a.max_z, b.min_z, b.max_z);
  if (!overlap_z.has_value()) {
    return absl::nullopt;
  }

  Cube r;
  r.min_x = overlap_x->first;
  r.max_x = overlap_x->second;
  r.min_y = overlap_y->first;
  r.max_y = overlap_y->second;
  r.min_z = overlap_z->first;
  r.max_z = overlap_z->second;

  return r;
}

using XYZ = std::array<int, 6>;

int main(int argc, char** argv) {
  std::string input = readAll();

  std::vector<Cube> cubes;

  for (absl::string_view s : absl::StrSplit(input, '\n', absl::SkipEmpty())) {
    bool new_state = absl::StartsWith(s, "on");
    std::string line;
    if (new_state) {
      line = std::string(s.substr(3));
    } else {
      line = std::string(s.substr(4));
    }
    Cube& c = cubes.emplace_back();
    c.on = new_state;
    sscanf(line.c_str(), "x=%ld..%ld,y=%ld..%ld,z=%ld..%ld", &c.min_x, &c.max_x,
           &c.min_y, &c.max_y, &c.min_z, &c.max_z);
  }

  std::vector<Cube> existing;
  for (const Cube& c : cubes) {
    std::vector<Cube> next = existing;
    for (const Cube& e : existing) {
      auto intersection = overlap(c, e);
      if (intersection.has_value()) {
        next.emplace_back(*intersection);
        next.back().on = !e.on;
      }
    }
    existing = std::move(next);

    if (c.on) {
      existing.push_back(c);
    }
  }

  long volume = 0;
  for (const Cube& c : existing) {
    if (c.on) {
      volume += c.Volume();
    } else {
      const long v = c.Volume();
      assert(volume >= v);
      volume -= v;
    }
  }

  absl::PrintF("%d\n", volume);
}
