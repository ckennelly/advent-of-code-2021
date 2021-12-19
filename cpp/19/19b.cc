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

#include "19/common.h"
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

using namespace aoc2021_19;

struct XYZ {
  int x;
  int y;
  int z;

  template <typename H>
  friend H AbslHashValue(H h, const XYZ& t) {
    return H::combine(std::move(h), t.x, t.y, t.z);
  }

  friend bool operator==(const XYZ& lhs, const XYZ& rhs) {
    return std::tie(lhs.x, lhs.y, lhs.z) == std::tie(rhs.x, rhs.y, rhs.z);
  }
};

XYZ permute(XYZ input, int x) {
  // Half of these permutations are nominally redundant.
  assert(x < 48);

  int updown = x / 6;
  assert(updown < 8);
  if (updown & 0x1) {
    input.x *= -1;
  }
  if (updown & 0x2) {
    input.y *= -1;
  }
  if (updown & 0x4) {
    input.z *= -1;
  }

  XYZ output = input;
  int shuffle = x % 6;
  using std::swap;
  switch (shuffle) {
    case 0:  // xyz
      break;
    case 1:  // yxz
      swap(output.x, output.y);
      break;
    case 2:  // zyx
      swap(output.x, output.z);
      break;
    case 3:  // xzy
      swap(output.y, output.z);
      break;
    case 4:  // yzx
      output.x = input.y;
      output.y = input.z;
      output.z = input.x;
      break;
    case 5:  // zxy
      output.x = input.z;
      output.y = input.x;
      output.z = input.y;
      break;
    default:
      __builtin_unreachable();
  }

  return output;
}

int main(int argc, char** argv) {
  std::string input = readAll();
  std::vector<std::string> scannerInput =
      absl::StrSplit(input, "\n\n", absl::SkipEmpty());

  std::vector<std::vector<XYZ>> scannerBeacons;
  for (const auto& scannerPacket : scannerInput) {
    std::vector<std::string> lines =
        absl::StrSplit(scannerPacket, "\n", absl::SkipEmpty());
    assert(!lines.empty());
    assert(lines[0] ==
           absl::StrFormat("--- scanner %d ---", scannerBeacons.size()));

    scannerBeacons.emplace_back();
    for (int i = 1; i < lines.size(); i++) {
      auto& beacons = scannerBeacons.back().emplace_back();
      sscanf(lines[i].c_str(), "%d,%d,%d", &beacons.x, &beacons.y, &beacons.z);
    }
  }

  std::vector<std::vector<XYZ>> knownBeacons;
  knownBeacons.push_back(scannerBeacons[0]);
  std::vector<XYZ> knownScanners;
  knownScanners.push_back({0, 0, 0});
  std::vector<int> knownPermutations;
  knownPermutations.push_back(0);
  std::vector<int> knownIndex;
  knownIndex.push_back(0);

  absl::flat_hash_set<int> unsolved;
  for (int i = 1; i < scannerBeacons.size(); i++) {
    unsolved.insert(i);
  }

  while (!unsolved.empty()) {
    const size_t unsolved_top = unsolved.size();
    for (const int index : unsolved) {
      assert(knownBeacons.size() == knownScanners.size());
      assert(knownScanners.size() == knownPermutations.size());
      assert(knownPermutations.size() == knownIndex.size());

      const auto& currentBeacons = scannerBeacons[index];
      bool found = false;

      for (int perm = 0; perm < 48; perm++) {
        absl::flat_hash_set<XYZ> points;
        for (auto beacon : currentBeacons) {
          const XYZ permuted = permute(beacon, perm);
          points.insert(permuted);
        }
        assert(points.size() >= 12);

        for (int i = 0; i < knownBeacons.size(); i++) {
          for (XYZ starting : knownBeacons[i]) {
            for (XYZ unknown : points) {
              XYZ diff = {starting.x - unknown.x, starting.y - unknown.y,
                          starting.z - unknown.z};

              int matches = 1;  // By definition
              for (XYZ other : knownBeacons[i]) {
                if (other == starting) {
                  continue;
                }

                XYZ new_diff = {other.x - diff.x, other.y - diff.y,
                                other.z - diff.z};
                if (points.contains(new_diff)) {
                  matches++;
                }
              }

              if (matches >= 12) {
                XYZ actual = diff;
                knownScanners.push_back(actual);
                knownIndex.push_back(index);

                auto& newBeacons = knownBeacons.emplace_back();
                newBeacons.reserve(points.size());
                for (XYZ point : points) {
                  XYZ rewritten{point.x + diff.x, point.y + diff.y,
                                point.z + diff.z};
                  newBeacons.push_back(rewritten);
                }
                found = true;
                break;
              }
            }

            if (found) {
              break;
            }
          }

          if (found) {
            break;
          }
        }

        if (found) {
          knownPermutations.emplace_back(perm);
          break;
        }
      }

      assert(knownBeacons.size() == knownScanners.size());
      assert(knownScanners.size() == knownPermutations.size());

      if (found) {
        unsolved.erase(index);
        break;
      }
    }

    if (unsolved.size() == unsolved_top) {
      abort();
    }
  }

  int best = INT_MIN;
  for (auto& xyz1 : knownScanners) {
    for (auto& xyz2 : knownScanners) {
      best =
          std::max(best, std::abs(xyz1.x - xyz2.x) + std::abs(xyz1.y - xyz2.y) +
                             std::abs(xyz1.z - xyz2.z));
    }
  }
  absl::FPrintF(stdout, "%d\n", best);
}
