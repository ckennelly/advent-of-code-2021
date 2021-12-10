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

  std::vector<int64_t> scores;
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

    if (mismatch != 0) {
      // Corrupted
      continue;
    }

    int64_t line_score = 0;
    for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
      for (int i = 0; i < it->second; i++) {
        line_score *= 5;
        switch (it->first) {
          case '(':
            line_score += 1;
            break;
          case '[':
            line_score += 2;
            break;
          case '{':
            line_score += 3;
            break;
          case '<':
            line_score += 4;
            break;
        }
      }
    }

    scores.push_back(line_score);
  }

  std::sort(scores.begin(), scores.end());

  printf("%lld\n", scores[scores.size() / 2]);
}
