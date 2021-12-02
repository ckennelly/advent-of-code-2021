#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>

#include "absl/strings/string_view.h"

int main(int argc, char** argv) {
  int x = 0, y = 0, aim = 0;

  char action[256];
  int value;
  while (fscanf(stdin, "%s %d", action, &value) == 2) {
    absl::string_view a(action);
    if (a == "forward") {
      x += value;
      y += aim * value;
    } else if (a == "down") {
      aim += value;
    } else if (a == "up") {
      aim -= value;
    } else {
      assert(false);
    }
  }

  printf("%d\n", std::abs(x * y));
}
