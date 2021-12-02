#include <cstdio>
#include <vector>

int main(int argc, char** argv) {
  std::vector<int> values;
  int value;
  while (fscanf(stdin, "%d", &value) == 1) {
    values.push_back(value);
  }

  int increases = 0;
  for (int i = 1; i < values.size(); i++) {
    if (values[i - 1] < values[i]) {
      increases++;
    }
  }

  printf("%d\n", increases);
}
