#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

class Board {
 public:
  Board() { memset(marked, 0, sizeof(marked)); }

  // returns nullopt if not a winner yet
  absl::optional<int> mark(int number) {
    if (won) {
      return absl::nullopt;
    }

    bool found = false;
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        if (value[i][j] == number) {
          marked[i][j] = true;
          found = true;
        }
      }
    }

    if (!found) {
      return absl::nullopt;
    }

    bool winner = false;
    for (int i = 0; i < 5; i++) {
      bool column = true, row = true;

      for (int j = 0; j < 5; j++) {
        column &= marked[i][j];
        row &= marked[j][i];
      }

      if (column || row) {
        winner = true;
        break;
      }
    }

    if (!winner) {
      return absl::nullopt;
    }

    int unmarked = 0;
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        if (!marked[i][j]) {
          unmarked += value[i][j];
        }
      }
    }

    won = true;
    return unmarked * number;
  }

  int value[5][5];
  bool marked[5][5];
  bool won = false;
};

int main(int argc, char** argv) {
  std::vector<int> numbers;
  std::vector<Board> boards;

  char buf[512];
  fscanf(stdin, "%s", buf);
  for (absl::string_view sv : absl::StrSplit(buf, ',')) {
    // Guaranteed to be null terminated
    int value;
    sscanf(sv.data(), "%d", &value);

    numbers.push_back(value);
  }

  int v1, v2, v3, v4, v5;
  int row = 0;
  while (fscanf(stdin, "%d %d %d %d %d", &v1, &v2, &v3, &v4, &v5) == 5) {
    if (row == 0) {
      boards.emplace_back();
    }

    boards.back().value[row][0] = v1;
    boards.back().value[row][1] = v2;
    boards.back().value[row][2] = v3;
    boards.back().value[row][3] = v4;
    boards.back().value[row][4] = v5;

    row++;
    if (row == 5) {
      row = 0;
    }
  }

  assert(row == 0);

  // Run game
  int winners = 0;
  for (int number : numbers) {
    for (auto& board : boards) {
      auto result = board.mark(number);
      if (!result.has_value()) {
        continue;
      }

      winners++;

      if (winners == boards.size()) {
        printf("%d\n", *result);
        return 0;
      }
    }
  }

  assert(false);
}
