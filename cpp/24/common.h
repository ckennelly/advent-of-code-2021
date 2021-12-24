#ifndef AOC_2021_24_COMMON_H_
#define AOC_2021_24_COMMON_H_

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

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "absl/strings/numbers.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/types/optional.h"
#include "absl/types/variant.h"

namespace aoc2021_24 {

std::string readAll() {
  std::stringstream buffer;
  buffer << std::cin.rdbuf();
  return buffer.str();
}

struct State {
  std::array<long, 4> values = {0, 0, 0, 0};
};

enum class OpCode {
  Input,
  Add,
  Mul,
  Div,
  Mod,
  Equal,
};

enum Register {
  W = 0,
  X = 1,
  Y = 2,
  Z = 3,
};

struct Op {
  OpCode op;
  Register a;
  absl::variant<Register, long> b;
};

long GetValue(const State& s, const absl::variant<Register, long>& r) {
  if (absl::holds_alternative<long>(r)) {
    return absl::get<long>(r);
  }

  assert(absl::holds_alternative<Register>(r));
  return s.values[absl::get<Register>(r)];
}

// Offset of input
std::pair<State, int> DoOp(const std::pair<State, int>& in,
                           absl::Span<char> digits, const Op& o) {
  State out = in.first;
  int count = in.second;
  switch (o.op) {
    case OpCode::Input:
      out.values[o.a] = digits[count];
      count++;
      break;
    case OpCode::Add:
      out.values[o.a] += GetValue(in.first, o.b);
      break;
    case OpCode::Mul:
      out.values[o.a] *= GetValue(in.first, o.b);
      break;
    case OpCode::Div:
      out.values[o.a] /= GetValue(in.first, o.b);
      break;
    case OpCode::Mod:
      out.values[o.a] %= GetValue(in.first, o.b);
      break;
    case OpCode::Equal:
      out.values[o.a] = long(out.values[o.a] == GetValue(in.first, o.b));
      break;
    default:
      assert(false);
  }
  return {out, count};
}

long RunProgram(absl::Span<const Op> program, absl::Span<char> digits) {
  std::pair<State, int> s;
  for (const Op& op : program) {
    s = DoOp(s, digits, op);
  }
  assert(s.second <= digits.size());
  return s.first.values[Z];
}

long ToInt(absl::string_view s) {
  long a;
  bool b = absl::SimpleAtoi(s, &a);
  (void)b;
  assert(b);
  return a;
}

std::vector<Op> ParseProgram(absl::string_view input) {
  const absl::flat_hash_map<absl::string_view, OpCode> mapping = {
      {"inp", OpCode::Input}, {"add", OpCode::Add}, {"mul", OpCode::Mul},
      {"div", OpCode::Div},   {"mod", OpCode::Mod}, {"eql", OpCode::Equal},
  };

  std::vector<Op> program;
  const std::regex r("^([a-z]+) ([a-z])( [a-z0-9-]+)?$");

  for (const absl::string_view l :
       absl::StrSplit(input, '\n', absl::SkipEmpty())) {
    const std::string line(l);
    std::smatch matches;
    bool ret = std::regex_match(line, matches, r);
    (void)ret;
    assert(ret);
    assert(matches.size() >= 3);

    Op& o = program.emplace_back();
    o.op = mapping.at(matches[1].str());
    const std::string a = matches[2].str();
    assert(a.size() == 1);
    assert(a[0] >= 'w');
    assert(a[0] <= 'z');
    o.a = static_cast<Register>(a[0] - 'w');

    const std::string b(absl::StripAsciiWhitespace(matches[3].str()));
    if (b.empty()) {
      assert(o.op == OpCode::Input);
    } else {
      long int_b;
      if (absl::SimpleAtoi(b, &int_b)) {
        o.b = int_b;
      } else {
        assert(b.size() == 1);
        assert(b[0] >= 'w');
        assert(b[0] <= 'z');
        o.b = static_cast<Register>(b[0] - 'w');
      }
    }
  }
  return program;
}

std::vector<std::tuple<long, long, long>> ParseSummary(
    absl::string_view input) {
  const std::regex segment_regex(R"(mul x 0
add x z
mod x 26
div z ([-0-9]+)
add x ([-0-9]+)
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y ([-0-9]+)
mul y x
add z y)");

  std::vector<std::tuple<long, long, long>> summary;

  for (const absl::string_view s :
       absl::StrSplit(input, "inp w", absl::SkipEmpty())) {
    const std::string segment(absl::StripAsciiWhitespace(s));
    std::smatch matches;
    bool ret = std::regex_match(segment, matches, segment_regex);
    (void)ret;
    assert(ret);
    assert(matches.size() == 4);

    auto& abc = summary.emplace_back();
    std::get<0>(abc) = ToInt(matches[1].str());
    std::get<1>(abc) = ToInt(matches[2].str());
    std::get<2>(abc) = ToInt(matches[3].str());
  }

  assert(summary.size() == 14);
  return summary;
}

absl::flat_hash_map<int, std::tuple<int, long>> FindRelationships(
    const std::vector<std::tuple<long, long, long>>& summary) {
  std::vector<std::tuple<int, long>> stack;
  // a[first] = a[second.first] + a.second.second
  absl::flat_hash_map<int, std::tuple<int, long>> relationships;
  for (int i = 0; i < summary.size(); i++) {
    const auto& [a, b, c] = summary[i];
    if (a == 1) {
      // Push
      stack.emplace_back(i, c);
    } else if (a == 26) {
      // Pop
      assert(!stack.empty());

      auto [top, old_c] = stack.back();
      stack.pop_back();

      // a[top] + old_c = a[i] - b
      // a[top] = a[i] - old_c - b
      bool inserted =
          relationships.emplace(top, std::make_tuple(i, -old_c - b)).second;
      (void)inserted;
      assert(inserted);
    } else {
      assert(false);
    }
  }
  return relationships;
}

}  // namespace aoc2021_24

#endif  // AOC_2021_24_COMMON_H_
