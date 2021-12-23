#ifndef AOC_2021_23_COMMON_H_
#define AOC_2021_23_COMMON_H_

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

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"
#include "absl/types/variant.h"

namespace aoc2021_23 {

enum Spot {
  A = 'A',
  B = 'B',
  C = 'C',
  D = 'D',
  Empty = '.',
};

inline int animalCost(Spot s) {
  switch (s) {
    case A:
      return 1;
    case B:
      return 10;
    case C:
      return 100;
    case D:
      return 1000;
    default:
      assert(false);
      return 0;
  }
}

template <int Depth>
struct StateImpl {
  static constexpr int kDepth = Depth;

  std::array<Spot, 7> hallway = {Empty, Empty, Empty, Empty,
                                 Empty, Empty, Empty};
  std::array<std::array<Spot, Depth>, 4> burrows;

  template <typename H>
  friend H AbslHashValue(H h, const StateImpl& t) {
    return H::combine(std::move(h), t.hallway, t.burrows);
  }

  friend bool operator==(const StateImpl& l, const StateImpl& r) {
    return l.hallway == r.hallway && l.burrows == r.burrows;
  }

  bool Solved() const {
    for (const Spot s : hallway) {
      if (s != Empty) {
        return false;
      }
    }

    for (int i = 0; i < 4; i++) {
      for (const Spot s : burrows[i]) {
        if (s != 'A' + i) {
          return false;
        }
      }
    }

    return true;
  }
};

template <int Depth>
inline void Print(const StateImpl<Depth>& s) {
  absl::FPrintF(stderr, "#############\n");
  absl::FPrintF(stderr, "#%c%c.%c.%c.%c.%c%c#\n", s.hallway[0], s.hallway[1],
                s.hallway[2], s.hallway[3], s.hallway[4], s.hallway[5],
                s.hallway[6]);
  absl::FPrintF(stderr, "###%c#%c#%c#%c###\n", s.burrows[0][0], s.burrows[1][0],
                s.burrows[2][0], s.burrows[3][0]);
  for (int i = 1; i < s.burrows[0].size(); i++) {
    absl::FPrintF(stderr, "  #%c#%c#%c#%c#  \n", s.burrows[0][i],
                  s.burrows[1][i], s.burrows[2][i], s.burrows[3][i]);
  }
  absl::FPrintF(stderr, "  #########  \n");
}

static constexpr int hallwayX[7] = {0, 1, 3, 5, 7, 9, 10};
static constexpr int burrowsX[4] = {2, 4, 6, 8};

// Includes +1 to *enter* the burrow.
static constexpr int hallBurrowDist[7][4] = {
  // clang-format: off
  {3, 5, 7, 9},
  {2, 4, 6, 8},
  {2, 2, 4, 6},
  {4, 2, 2, 4},
  {6, 4, 2, 2},
  {8, 6, 4, 2},
  {9, 7, 5, 3},
  // clang-format: on
};

// Return candidate set of states to visit next.
template <typename State>
std::vector<std::pair<State, int>> nextStates(const State& s) {
  std::vector<std::pair<State, int>> next;
  for (int hall = 0; hall < 7; hall++) {
    if (s.hallway[hall] == Empty) {
      continue;
    }

    const Spot animal = s.hallway[hall];

    // Animals can move out of the hallway, but can't move within it.
    do {
      int burrow = animal - 'A';
      assert(burrow >= 0);
      assert(burrow <= 3);

      int depth = 0;
      while (depth < State::kDepth && s.burrows[burrow][depth] == Empty) {
        depth++;
      }
      if (depth == 0) {
        // Can't be a candidate burrow.
        assert(s.burrows[burrow][0] != Empty);
        continue;
      }
      depth--;
      assert(depth >= 0);
      assert(depth < State::kDepth);

      bool would_block = false;
      for (int i = depth + 1; i < State::kDepth; i++) {
        if (s.burrows[burrow][i] != Empty && s.burrows[burrow][i] != animal) {
          // Would block in the wrong animal.
          would_block = true;
          break;
        }
      }
      if (would_block) {
        continue;
      }

      // Can we get there?
      const int hallX = hallwayX[hall];
      const int destX = burrowsX[burrow];
      bool success = true;
      if (hallX < destX) {
        for (int cur = hall + 1; cur < 7 && hallwayX[cur] < destX; cur++) {
          if (s.hallway[cur] != Empty) {
            success = false;
            break;
          }
        }
      } else {
        assert(destX < hallX);
        for (int cur = hall - 1; cur >= 0 && hallwayX[cur] > destX; cur--) {
          if (s.hallway[cur] != Empty) {
            success = false;
            break;
          }
        }
      }

      State r = s;
      r.hallway[hall] = Empty;
      int dist = hallBurrowDist[hall][burrow];

      // Going into the burrow further.
      dist += depth;
      r.burrows[burrow][depth] = animal;

      if (!success) {
        continue;
      }

      next.emplace_back(r, animalCost(animal) * dist);
    } while (false);
  }

  // For each burrow, consider a move to the hallway or another burrow.
  for (int start = 0; start < 4; start++) {
    Spot animal = Empty;
    bool all_empty = true;
    int from_depth;
    for (from_depth = 0; from_depth < State::kDepth; from_depth++) {
      all_empty = all_empty && s.burrows[start][from_depth] == Empty;

      if (s.burrows[start][from_depth] != Empty) {
        animal = s.burrows[start][from_depth];
        break;
      }
    }
    if (all_empty) {
      continue;
    }

    assert(animal != Empty);
    assert(from_depth >= 0);
    assert(from_depth < State::kDepth);

    // "Burrow to burrow" is equivalent to "burrow to hallway to burrow", so we
    // elide that.

    // Do hallway moves
    for (int hall = 0; hall < 7; hall++) {
      if (s.hallway[hall] != Empty) {
        continue;
      }

      // Is it clear?
      bool success = true;
      if (hallwayX[hall] < burrowsX[start]) {
        for (int i = hall; i < 7 && hallwayX[i] < burrowsX[start]; i++) {
          if (s.hallway[i] != Empty) {
            success = false;
          }
        }
      } else {
        for (int i = hall; i >= 0 && hallwayX[i] > burrowsX[start]; i--) {
          if (s.hallway[i] != Empty) {
            success = false;
          }
        }
      }

      if (!success) {
        continue;
      }

      State r = s;
      r.hallway[hall] = animal;
      r.burrows[start][from_depth] = Empty;

      int dist = hallBurrowDist[hall][start];
      // Going up the burrow further.
      dist += from_depth;

      next.emplace_back(r, animalCost(animal) * dist);
    }
  }

  return next;
}

}  // namespace aoc2021_23

#endif  // AOC_2021_23_COMMON_H_
