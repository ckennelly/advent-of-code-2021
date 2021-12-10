#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"

std::string readAll() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  return buffer.str();
}

std::vector<int> SplitToInts(absl::string_view sv, absl::string_view delim) {
  std::vector<int> r;
  for (absl::string_view chunk :
       absl::StrSplit(sv, delim, absl::SkipWhitespace())) {
    int a;
    if (absl::SimpleAtoi(chunk, &a)) {
      r.push_back(a);
    } else {
      assert(false);
    }
  }
  return r;
}

int main(int argc, char** argv) {
  std::string input = readAll();
  std::vector<std::string> lines =
      absl::StrSplit(input, '\n', absl::SkipEmpty());

  int scores = 0;
  for (const auto& line : lines) {
    std::vector<std::pair<char, int>> stack;

    char mismatch = 0;
    for (const char c : line) {
      switch (c) {
        case '(':
        case '[':
        case '{':
        case '<':
          if (stack.empty() || stack.back().first != c) {
            stack.emplace_back();
            stack.back().first = c;
          }

          stack.back().second++;
          break;
        case ')':
          if (stack.empty() || stack.back().first != '(') {
            mismatch = c;
            break;
          }

          assert(stack.back().second >= 1);
          stack.back().second--;
          break;
        case ']':
          if (stack.empty() || stack.back().first != '[') {
            mismatch = c;
            break;
          }

          assert(stack.back().second >= 1);
          stack.back().second--;
          break;

        case '}':
          if (stack.empty() || stack.back().first != '{') {
            mismatch = c;
            break;
          }

          assert(stack.back().second >= 1);
          stack.back().second--;
          break;

        case '>':
          if (stack.empty() || stack.back().first != '<') {
            mismatch = c;
            break;
          }

          assert(stack.back().second >= 1);
          stack.back().second--;
          break;

          break;
        default:
          assert(false);
      }

      if (stack.back().second == 0) {
        stack.pop_back();
      }

      if (mismatch != 0) {
        break;
      }
    }

    switch (mismatch) {
      case ')':
        scores += 3;
        break;
      case ']':
        scores += 57;
        break;
      case '}':
        scores += 1197;
        break;
      case '>':
        scores += 25137;
        break;
      default:
        break;
    }
  }

  printf("%d\n", scores);
}
