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
#include <tuple>
#include <vector>

#include "21/common.h"
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

using namespace aoc2021_21;

using State = std::tuple<int, int, int, int, int>;

int main(int argc, char** argv) {
  std::string input = readAll();
  std::map<int, int> positions;
  for (absl::string_view line :
       absl::StrSplit(input, '\n', absl::SkipEmpty())) {
    int player, pos;
    sscanf(line.data(), "Player %d starting position: %d", &player, &pos);
    positions[player - 1] = pos - 1;
  }

  uint64_t roll_dist[10] = {0};
  for (int i = 1; i <= 3; i++) {
    for (int j = 1; j <= 3; j++) {
      for (int k = 1; k <= 3; k++) {
        roll_dist[i + j + k]++;
      }
    }
  }

  absl::flat_hash_map<State, uint64_t> ways_to_score;

  absl::flat_hash_map<State, uint64_t> to_visit;

  State starting;
  std::get<0>(starting) = positions[0];
  std::get<1>(starting) = positions[1];
  to_visit.emplace(starting, 1);

  while (!to_visit.empty()) {
    absl::flat_hash_map<State, uint64_t> new_visit;

    for (const auto& kv : to_visit) {
      const auto& state = kv.first;
      const uint64_t count = kv.second;
      ways_to_score[state] += count;

      if (std::get<2>(state) >= 21 || std::get<3>(state) >= 21) {
        continue;
      }

      const int old_turn = std::get<4>(state);

      for (int i = 0; i < 10; i++) {
        if (roll_dist[i] == 0) {
          continue;
        }
        State s = state;
        std::get<4>(s) ^= 1;

        int& pos = old_turn == 0 ? std::get<0>(s) : std::get<1>(s);
        int& score = old_turn == 0 ? std::get<2>(s) : std::get<3>(s);

        pos = (pos + i) % 10;
        score += pos + 1;

        new_visit[s] += count * roll_dist[i];
      }
    }

    to_visit = std::move(new_visit);
  }

  uint64_t player_counts[2] = {0, 0};
  for (auto& way_to_score : ways_to_score) {
    bool winners[2] = {std::get<2>(way_to_score.first) >= 21,
                       std::get<3>(way_to_score.first) >= 21};
    if (!winners[0] && !winners[1]) {
      continue;
    }

    player_counts[std::get<4>(way_to_score.first) ^ 1] += way_to_score.second;
  }

  absl::PrintF("%u\n", std::max(player_counts[0], player_counts[1]));
}
