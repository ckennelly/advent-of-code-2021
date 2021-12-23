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

#include "23/common.h"
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

using namespace aoc2021_23;
using State = aoc2021_23::StateImpl<2>;

int main(int argc, char** argv) {
  State starting;
  if (argc > 1 && strcmp(argv[1], "test") == 0) {
    // Test
    starting.burrows[0][0] = B;
    starting.burrows[0][1] = A;
    starting.burrows[1][0] = C;
    starting.burrows[1][1] = D;
    starting.burrows[2][0] = B;
    starting.burrows[2][1] = C;
    starting.burrows[3][0] = D;
    starting.burrows[3][1] = A;
  } else {
    starting.burrows[0][0] = A;
    starting.burrows[0][1] = C;
    starting.burrows[1][0] = D;
    starting.burrows[1][1] = C;
    starting.burrows[2][0] = A;
    starting.burrows[2][1] = D;
    starting.burrows[3][0] = B;
    starting.burrows[3][1] = B;
  }

  absl::flat_hash_map<State, int> visited;
  std::map<int, std::vector<State>> to_visit;
  to_visit[0].emplace_back(starting);

  absl::optional<State> solution;

  while (!to_visit.empty() && !solution.has_value()) {
    const auto& [cost, visiting] = *to_visit.begin();
    for (const auto& to : visiting) {
      if (!visited.emplace(to, cost).second) {
        // Already visited
        assert(!to.Solved());
        continue;
      }

      if (to.Solved()) {
        solution = to;
        printf("%d\n", cost);
        return 0;
      }

      std::vector<std::pair<State, int>> next = nextStates(to);
      for (const auto& [next_state, delta] : next) {
        assert(delta > 0);
        to_visit[cost + delta].emplace_back(next_state);
      }
    }

    to_visit.erase(to_visit.begin());
  }

  assert(solution.has_value());
}
