#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::array<int, 12> bits = {};
  std::string pattern;
  while (input >> pattern) {
    if (pattern.size() != 12) {
      std::cout << "error";
      exit(-1);
    }
    for (size_t i = 0; i < pattern.size(); ++i)
      bits[i] += -1 + 2 * (pattern[i] == '1');
  }
  size_t result = 0;
  for (size_t i = 0; i < pattern.size(); ++i)
    result += (bits[i] > 0) << (11 - i);

  std::cout << "Result " << std::dec << result * (~result & 0x0FFF) << "\n";
  return 0;
}
