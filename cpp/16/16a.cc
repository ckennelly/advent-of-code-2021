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

uint64_t consumeInt(absl::string_view& s, size_t size) {
  assert(s.size() >= size);
  uint64_t value = 0;
  for (int i = 0; i < size; i++) {
    value <<= 1;
    value += s[i] - '0';
  }
  s = s.substr(size);
  return value;
}

uint64_t consumeVersion(absl::string_view& s) {
  return consumeInt(s, 3);
}

uint64_t consumeType(absl::string_view& s) {
  return consumeInt(s, 3);
}

uint64_t consumeVarint(absl::string_view& s) {
  uint64_t result = 0;
  uint64_t next;
  do {
    next = consumeInt(s, 5);
    result <<= 4;
    result += next & 0xF;
  } while (next & 0x10);
  return result;
}

std::string convert(std::string s) {
  std::string ret;
  ret.reserve(s.size() * 4);

  absl::flat_hash_map<char, std::string> m = {
   {'0', "0000"},
   {'1', "0001"},
   {'2', "0010"},
   {'3', "0011"},
   {'4', "0100"},
   {'5', "0101"},
   {'6', "0110"},
   {'7', "0111"},
   {'8', "1000"},
   {'9', "1001"},
   {'A', "1010"},
   {'B', "1011"},
   {'C', "1100"},
   {'D', "1101"},
   {'E', "1110"},
   {'F', "1111"},
  };

  for (auto c : s) {
    ret.append(m[c]);
  }
  return ret;
}

uint64_t parsePacket(absl::string_view& s) {
  const uint64_t version = consumeVersion(s);
  const uint64_t type = consumeType(s);

  uint64_t total_sum = version;

  switch (type) {
    case 4: {
      consumeVarint(s);
      break;
    }
    default: {
      const auto lengthId = consumeInt(s, 1);
      switch (lengthId) {
        case 0: {
          const int bytes = consumeInt(s, 15);
          absl::string_view substr = s.substr(0, bytes);
          s = s.substr(bytes);

          while (!substr.empty()) {
            total_sum += parsePacket(substr);
          }

          break;
        }
        case 1: {
          const int count = consumeInt(s, 11);
          for (int i = 0; i < count; i++) {
            total_sum += parsePacket(s);
          }
          break;
        }
        default:
          assert(false);
      }
    }
  }

  return total_sum;
}

int main(int argc, char** argv) {
  std::string input = readAll();
  std::string hex2bin = convert(input);
  absl::string_view s = hex2bin;

  uint64_t total_sum = parsePacket(s);

  printf("%lu\n", total_sum);
}
