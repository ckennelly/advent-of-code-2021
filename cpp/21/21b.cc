#include <algorithm>
#include <array>
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

using State = std::tuple<std::array<int, 2>, std::array<int, 2>, int>;

int& position(State& s, int player) { return std::get<0>(s)[player]; }

int& score(State& s, int player) { return std::get<1>(s)[player]; }

const int& score(const State& s, int player) { return std::get<1>(s)[player]; }

int& turn(State& s) { return std::get<2>(s); }

const int& turn(const State& s) { return std::get<2>(s); }

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
  position(starting, 0) = positions[0];
  position(starting, 1) = positions[1];
  to_visit.emplace(starting, 1);

  while (!to_visit.empty()) {
    absl::flat_hash_map<State, uint64_t> new_visit;

    for (const auto& kv : to_visit) {
      const auto& state = kv.first;
      const uint64_t count = kv.second;
      ways_to_score[state] += count;

      if (score(state, 0) >= 21 || score(state, 1) >= 21) {
        continue;
      }

      for (int i = 0; i < 10; i++) {
        if (roll_dist[i] == 0) {
          continue;
        }
        State s = state;
        int& current = turn(s);

        int& pos = position(s, current);
        int& player_score = score(s, current);

        pos = (pos + i) % 10;
        player_score += pos + 1;
        current ^= 1;

        new_visit[s] += count * roll_dist[i];
      }
    }

    to_visit = std::move(new_visit);
  }

  uint64_t player_counts[2] = {0, 0};
  for (auto& way_to_score : ways_to_score) {
    bool winners[2] = {score(way_to_score.first, 0) >= 21,
                       score(way_to_score.first, 1) >= 21};
    if (!winners[0] && !winners[1]) {
      continue;
    }

    // Both players should not have won.
    assert(int(winners[0]) ^ int(winners[1]));
    player_counts[turn(way_to_score.first) ^ 1] += way_to_score.second;
  }

  absl::PrintF("%u\n", std::max(player_counts[0], player_counts[1]));
}
