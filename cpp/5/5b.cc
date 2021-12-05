#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

#include "absl/strings/numbers.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

struct Line {
  int x1;
  int y1;
  int x2;
  int y2;
};

int main(int argc, char** argv) {
  std::vector<Line> lines;

  int max_x = 0, max_y = 0;

  int v1, v2, v3, v4;
  while (fscanf(stdin, "%d,%d -> %d,%d", &v1, &v2, &v3, &v4) == 4) {
    lines.push_back({v1, v2, v3, v4});

    max_x = std::max(max_x, std::max(v1, v3));
    max_y = std::max(max_y, std::max(v2, v4));
  }

  max_x++;
  max_y++;

  std::vector<std::vector<int>> grid;
  for (int i = 0; i < max_x; i++) {
    grid.push_back(std::vector<int>(max_y, 0));
  }

  for (auto line : lines) {
    if (line.x1 == line.x2) {
      for (int y = std::min(line.y1, line.y2); y <= std::max(line.y1, line.y2);
           y++) {
        grid[line.x1][y]++;
      }
    } else if (line.y1 == line.y2) {
      for (int x = std::min(line.x1, line.x2); x <= std::max(line.x1, line.x2);
           x++) {
        grid[x][line.y1]++;
      }
    } else {
      int direction_x = (line.x1 < line.x2) ? 1 : -1;
      int direction_y = (line.y1 < line.y2) ? 1 : -1;
      int x, y;
      for (x = line.x1, y = line.y1;
           x != (line.x2 + direction_x) && y != (line.y2 + direction_y);) {
        grid[x][y]++;

        x += direction_x;
        y += direction_y;
      }

      assert(x == line.x2 + direction_x);
      assert(y == line.y2 + direction_y);
    }
  }

  int count = 0;
  for (int y = 0; y < max_y; y++) {
    for (int x = 0; x < max_x; x++) {
      if (grid[x][y] > 1) {
        count++;
      }
    }
  }

  printf("%d\n", count);
}
