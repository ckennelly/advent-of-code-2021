#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>

int main(int argc, char** argv) {
  std::vector<int> counts;
  int count = 0;
  char buf[256];

  std::vector<unsigned> values;

  while (fscanf(stdin, "%s", buf) == 1) {
    size_t len = strlen(buf);
    if (len == 0) {
      continue;
    }
    counts.resize(len);

    unsigned value = 0;

    for (int i = 0; i < len; i++) {
      value <<= 1;

      switch (buf[i]) {
        case '0':
          break;
        case '1':
          value++;
          counts[i]++;
          break;
        default:
          assert(false);
      }
    }

    values.push_back(value);

    count++;
  }

  std::vector<unsigned> oxygen = values;
  std::vector<unsigned> co2 = values;

  for (int i = 0; i < counts.size(); i++) {
    unsigned offset = counts.size() - i - 1;
    unsigned mask = 1u << offset;

    unsigned ocount = 0, ccount = 0;
    for (auto u : oxygen) {
      ocount += (u & mask) >> offset;
    }

    for (auto u : co2) {
      ccount += (u & mask) >> offset;
    }

    unsigned expect_ox = ((ocount >= (oxygen.size() - ocount)) ? 1 : 0)
                         << offset;
    unsigned expect_co = ((ccount < (co2.size() - ccount)) ? 1 : 0) << offset;

    if (oxygen.size() > 1) {
      std::vector<unsigned> new_oxygen;
      for (auto u : oxygen) {
        if ((u & mask) == expect_ox) {
          new_oxygen.push_back(u);
        }
      }
      oxygen = std::move(new_oxygen);
    }

    if (co2.size() > 1) {
      std::vector<unsigned> new_co2;
      for (auto u : co2) {
        if ((u & mask) == expect_co) {
          new_co2.push_back(u);
        }
      }
      co2 = std::move(new_co2);
    }
  }

  assert(!oxygen.empty());
  assert(!co2.empty());

  printf("%d\n");
}
