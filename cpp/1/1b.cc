#include <cstdio>
#include <vector>

int main(int argc, char** argv) {
  std::vector<int> values;
  int value;
  while (fscanf(stdin, "%d", &value) == 1) {
    values.push_back(value);
  }

  int increases = 0;
  for (int i = 3; i < values.size(); i++) {
    int prior = values[i - 3] + values[i - 2] + values[i - 1];
    int after = values[i - 2] + values[i - 1] + values[i];

    if (prior < after) {
      increases++;
    }
  }

  printf("%d\n", increases);
}
