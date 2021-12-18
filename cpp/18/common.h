#ifndef AOC_2021_18_COMMON_H_
#define AOC_2021_18_COMMON_H_

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

#include "18/common.h"
#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"
#include "absl/types/variant.h"

namespace aoc2021_18 {

int parseInt(absl::string_view sv, absl::string_view context = {}) {
  int ret;
  bool success = absl::SimpleAtoi(sv, &ret);
  if (!success) {
    absl::FPrintF(stderr, "failed to parse: '%s' @ %s\n", sv, context);
    assert(success);
  }
  return ret;
}

void assertEq(absl::string_view l, absl::string_view r) {
#ifndef NDEBUG
  if (l != r) {
    absl::FPrintF(stderr, "'%s' != '%s'\n", l, r);
    assert(l != r);
  }
#endif  // NDEBUG
}

bool tryExplode(std::string& temp) {
  const std::regex left_regex("(.+?)([0-9]*)([^0-9]*)$");
  const std::regex right_regex("([0-9]+),([0-9]+)]([^0-9]*)([0-9]*)(.*)");

  int depth = 0;
  for (size_t i = 0; i < temp.size(); i++) {
    {
      char c = temp[i];
      switch (c) {
        case '[':
          depth++;
          break;
        case ']':
          assert(depth > 0);
          depth--;
          break;
      }
    }

    if (depth >= 5) {
      std::smatch left_match, right_match;

      const std::string full_left = std::string(temp.substr(0, i));
      const std::string full_right = std::string(temp.substr(i + 1));

      assertEq(temp, absl::StrCat(full_left, "[", full_right));

      const bool is_left_match =
          std::regex_match(full_left, left_match, left_regex);
      const bool is_right_match =
          std::regex_match(full_right, right_match, right_regex);
      (void)is_left_match;
      assert(is_left_match);
      (void)is_right_match;
      assert(is_right_match);

      assert(left_match.size() == 4);
      assert(right_match.size() == 6);

      assertEq(temp,
               absl::StrCat(left_match[1].str(), left_match[2].str(),
                            left_match[3].str(), "[", right_match[1].str(), ",",
                            right_match[2].str(), "]", right_match[3].str(),
                            right_match[4].str(), right_match[5].str()));

      const std::string far_left = left_match[1].str();
      const bool has_left = !left_match[2].str().empty();
      int next_left = 0;
      if (has_left) {
        next_left = parseInt(left_match[2].str(), "next_left");
      }
      const std::string left = left_match[3].str();
      // [
      int left_digit = parseInt(right_match[1].str(), "left_digit");
      // ,
      int right_digit = parseInt(right_match[2].str(), "right_digit");
      const std::string right = right_match[3].str();
      const bool has_right = !right_match[4].str().empty();
      int next_right = 0;
      if (has_right) {
        next_right = parseInt(right_match[4].str(), "next_right");
      }
      const std::string far_right = right_match[5].str();

      assertEq(temp, absl::StrCat(
                         far_left, has_left ? absl::StrCat(next_left) : "",
                         left, "[", left_digit, ",", right_digit, "]", right,
                         has_right ? absl::StrCat(next_right) : "", far_right));

      temp = absl::StrCat(
          far_left, has_left ? absl::StrCat(next_left + left_digit) : "", left,
          "0", right, has_right ? absl::StrCat(next_right + right_digit) : "",
          far_right);

#ifndef NDEBUG
      absl::FPrintF(stdout, "after explode: %s\n", temp);
#endif  // NDEBUG
      return true;
    }
  }

  return false;
}

void testExplode(absl::string_view input, absl::string_view output) {
  std::string copy(input);
  bool ret = tryExplode(copy);
  if (copy != output) {
    absl::FPrintF(stderr, "tryExplode(%s) = %s != %s\n", input, copy, output);
    assert(copy != output);
  }
  (void)ret;
  assert(ret == (input != output));
}

bool trySplit(std::string& temp) {
  const std::regex pieces_regex("(.*?)([0-9][0-9])(.*)");
  std::smatch pieces_match;

  if (std::regex_match(temp, pieces_match, pieces_regex)) {
    assert(pieces_match.size() == 4);
    std::string left = pieces_match[1].str();
    int num = parseInt(pieces_match[2].str());
    std::string right = pieces_match[3].str();

    int num_down = num / 2;
    int num_up = (num + 1) / 2;

    temp = absl::StrCat(left, "[", num_down, ",", num_up, "]", right);
#ifndef NDEBUG
    absl::FPrintF(stdout, "after split:   %s\n", temp);
#endif  // NDEBUG
    return true;
  }

  return false;
}

void testSplit(absl::string_view input, absl::string_view output) {
  std::string copy(input);
  bool ret = trySplit(copy);
  if (copy != output) {
    absl::FPrintF(stderr, "trySplit(%s) = %s != %s\n", input, copy, output);
    assert(copy != output);
  }
  (void)ret;
  assert(ret == (input != output));
}

void reduce(std::string& temp) {
  bool no_action;
  do {
    no_action = true;
    // Explode
    bool did_explode = tryExplode(temp);
    if (did_explode) {
      no_action = false;
      continue;
    }

    // Split
    bool did_split = trySplit(temp);
    if (did_split) {
      no_action = false;
      continue;
    }
  } while (!no_action);
}

void testReduce(absl::string_view input, absl::string_view output) {
  std::string copy(input);
  reduce(copy);
  if (copy != output) {
    absl::FPrintF(stderr, "reduce(%s) = %s != %s\n", input, copy, output);
    assert(copy != output);
  }
}

int magnitude(std::string line) {
  const std::regex pieces_regex("(.*?)\\[([0-9]+),([0-9]+)\\](.*)");
  std::smatch pieces_match;

  while (true) {
#ifndef NDEBUG
    absl::FPrintF(stdout, "magnitude reduction: %s\n", line);
#endif  // NDEBUG
    if (std::regex_match(line, pieces_match, pieces_regex)) {
      assert(pieces_match.size() == 5);

      const std::string left = pieces_match[1].str();
      const int left_digit = parseInt(pieces_match[2].str(), "mag left");
      const int right_digit = parseInt(pieces_match[3].str(), "mag right");
      const std::string right = pieces_match[4].str();

      int result = 3 * left_digit + 2 * right_digit;
      if (left.empty() && right.empty()) {
        return result;
      }

      line = absl::StrCat(left, result, right);
    } else {
      assert(false);
    }
  }
}

void testMagnitude(absl::string_view v, int expected) {
  int ret = magnitude(std::string(v));
  if (expected != ret) {
    absl::FPrintF(stderr, "magnitude(%s) = %d != %d\n", v, ret, expected);
    assert(expected != ret);
  }
}

namespace {

class SelfTester {
 public:
  SelfTester() {
#ifndef NDEBUG
    testExplode("[[[[[9,8],1],2],3],4]", "[[[[0,9],2],3],4]");
    testExplode("[7,[6,[5,[4,[3,2]]]]]", "[7,[6,[5,[7,0]]]]");
    testExplode("[[6,[5,[4,[3,2]]]],1]", "[[6,[5,[7,0]]],3]");
    testExplode("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]",
                "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");
    testExplode("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]",
                "[[3,[2,[8,0]]],[9,[5,[7,0]]]]");
    testSplit("[[[[0,7],4],[15,[0,13]]],[1,1]]",
              "[[[[0,7],4],[[7,8],[0,13]]],[1,1]]");
    testSplit("[[[[0,7],4],[[7,8],[0,13]]],[1,1]]",
              "[[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]");
    testExplode("[[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]",
                "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");

    // clang-format: off
    testReduce(
        "[[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]],[7,[[[3,7],[4,3]],[[6,3],["
        "8,8]]]]]",
        "[[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]]");
    testReduce(
        "[[[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]],[[2,[[0,8],"
        "[3,4]]],[[[6,7],1],[7,[1,6]]]]]",
        "[[[[6,7],[6,7]],[[7,7],[0,7]]],[[[8,7],[7,7]],[[8,8],[8,0]]]]");
    testReduce(
        "[[[[[6,7],[6,7]],[[7,7],[0,7]]],[[[8,7],[7,7]],[[8,8],[8,0]]]],[[[[2,"
        "4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]]",
        "[[[[7,0],[7,7]],[[7,7],[7,8]]],[[[7,7],[8,8]],[[7,7],[8,7]]]]");
    testReduce(
        "[[[[[7,0],[7,7]],[[7,7],[7,8]]],[[[7,7],[8,8]],[[7,7],[8,7]]]],[7,[5,["
        "[3,8],[1,4]]]]]",
        "[[[[7,7],[7,8]],[[9,5],[8,7]]],[[[6,8],[0,8]],[[9,9],[9,0]]]]");
    testReduce(
        "[[[[[7,7],[7,8]],[[9,5],[8,7]]],[[[6,8],[0,8]],[[9,9],[9,0]]]],[[2,[2,"
        "2]],[8,[8,1]]]]",
        "[[[[6,6],[6,6]],[[6,0],[6,7]]],[[[7,7],[8,9]],[8,[8,1]]]]");
    testReduce(
        "[[[[[6,6],[6,6]],[[6,0],[6,7]]],[[[7,7],[8,9]],[8,[8,1]]]],[2,9]]",
        "[[[[6,6],[7,7]],[[0,7],[7,7]]],[[[5,5],[5,6]],9]]");
    testReduce(
        "[[[[[6,6],[7,7]],[[0,7],[7,7]]],[[[5,5],[5,6]],9]],[1,[[[9,3],9],[[9,"
        "0],[0,7]]]]]",
        "[[[[7,8],[6,7]],[[6,8],[0,8]]],[[[7,7],[5,0]],[[5,5],[5,6]]]]");
    testReduce(
        "[[[[[7,8],[6,7]],[[6,8],[0,8]]],[[[7,7],[5,0]],[[5,5],[5,6]]]],[[[5,["
        "7,4]],7],1]]",
        "[[[[7,7],[7,7]],[[8,7],[8,7]]],[[[7,0],[7,7]],9]]");
    testReduce(
        "[[[[[7,7],[7,7]],[[8,7],[8,7]]],[[[7,0],[7,7]],9]],[[[[4,2],2],6],[8,"
        "7]]]",
        "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]");
    // clang-format: on

    testMagnitude("[9,1]", 29);
    testMagnitude("[1,9]", 21);
    testMagnitude("[[9,1],[1,9]]", 129);
    testMagnitude("[[1,2],[[3,4],5]]", 143);
    testMagnitude("[[[[0,7],4],[[7,8],[6,0]]],[8,1]]", 1384);
    testMagnitude("[[[[1,1],[2,2]],[3,3]],[4,4]]", 445);
    testMagnitude("[[[[3,0],[5,3]],[4,4]],[5,5]]", 791);
    testMagnitude("[[[[5,0],[7,4]],[5,5]],[6,6]]", 1137);
    testMagnitude("[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]",
                  3488);
#endif  // NDEBUG
  }
};

static SelfTester module_enter_exit;

}  // namespace
}  // namespace aoc2021_18

#endif  // AOC_2021_18_COMMON_H_
