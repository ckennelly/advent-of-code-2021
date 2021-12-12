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

std::vector<std::vector<std::string>> findPaths(
    const absl::flat_hash_map<std::string, std::vector<std::string>>& edges,
    const absl::flat_hash_set<std::string>& big,
    const absl::flat_hash_set<std::string>& small,
    const std::vector<std::string>& prefix,
    const absl::flat_hash_set<std::string>& small_visited) {
  assert(!prefix.empty());
  const auto& last = prefix.back();

  if (last == "end" || edges.at(last).empty()) {
    return {prefix};
  }

  std::vector<std::vector<std::string>> ret;
  for (auto next_edge : edges.at(last)) {
    std::vector<std::string> next_prefix = prefix;
    next_prefix.push_back(next_edge);
    absl::flat_hash_set<std::string> next_visited = small_visited;

    if (small.contains(next_edge) && !next_visited.insert(next_edge).second) {
      continue;
    }

    auto recurse = findPaths(edges, big, small, next_prefix, next_visited);
    ret.insert(ret.end(), recurse.begin(), recurse.end());
  }
  return ret;
}

int main(int argc, char** argv) {
  std::string input = readAll();
  std::vector<std::string> lines =
      absl::StrSplit(input, '\n', absl::SkipEmpty());

  absl::flat_hash_map<std::string, std::vector<std::string>> edges;
  absl::flat_hash_set<std::string> big;
  absl::flat_hash_set<std::string> small;

  auto split_big_small = [&](std::string arg) {
    assert(!arg.empty());
    if (arg[0] >= 'A' && arg[0] <= 'Z') {
      big.emplace(arg);
    } else {
      small.emplace(arg);
    }
  };

  for (const auto& line : lines) {
    std::vector<std::string> s = absl::StrSplit(line, '-');
    assert(s.size() == 2);
    edges[s[0]].push_back(s[1]);
    edges[s[1]].push_back(s[0]);

    split_big_small(s[0]);
    split_big_small(s[1]);
  }

  std::vector<std::string> prefix{"start"};
  absl::flat_hash_set<std::string> small_visited{"start"};
  std::vector<std::vector<std::string>> paths =
      findPaths(edges, big, small, prefix, small_visited);

  int count = 0;
  for (auto path : paths) {
    assert(!path.empty());

    if (path.back() == "end") {
      count++;
    }
  }

  printf("%d\n", count);
}
