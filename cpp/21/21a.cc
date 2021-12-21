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

int roll(int& die) {
  int ret = die;
  die++;
  if (die > 100) {
    die = 1;
  }
  return ret;
}

int main(int argc, char** argv) {
  std::string input = readAll();
  std::map<int, int> positions;
  for (absl::string_view line :
       absl::StrSplit(input, '\n', absl::SkipEmpty())) {
    int player, pos;
    // Safe due to input being null terminated.
    sscanf(line.data(), "Player %d starting position: %d", &player, &pos);
    positions[player - 1] = pos - 1;
  }

  int die = 1;
  int scores[2] = {0, 0};
  int turn = 0;
  int rolls = 0;
  while (scores[0] < 1000 && scores[1] < 1000) {
    int sum = roll(die) + roll(die) + roll(die);
    rolls += 3;

    int new_pos = (positions[turn] + sum) % 10;
    positions[turn] = new_pos;
    scores[turn] += new_pos + 1;
    turn = (turn + 1) % 2;
  }

  printf("%d\n", std::min(scores[0], scores[1]) * rolls);
}
