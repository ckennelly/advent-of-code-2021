#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

int main(int argc, char** argv) {
  std::vector<int> counts;
  int count = 0;
  char buf[256];

  while (fscanf(stdin, "%s", buf) == 1) {
    size_t len = strlen(buf);
    if (len == 0) {
      continue;
    }
    counts.resize(len);
    for (int i = 0; i < len; i++) {
      switch (buf[i]) {
        case '0':
          break;
        case '1':
          counts[i]++;
          break;
        default:
          assert(false);
      }
    }

    count++;
  }

  unsigned gamma = 0, epsilon = 0;
  for (int i = 0; i < counts.size(); i++) {
    gamma <<= 1;
    gamma += (counts[i] >= count / 2) ? 1 : 0;

    epsilon <<= 1;
    epsilon += (counts[i] < count / 2) ? 1 : 0;
  }

  printf("%d\n", gamma * epsilon);
}
