#include <algorithm>
#include <array>
#include <bitset>
#include <charconv>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <system_error>
#include <vector>

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::string line;
  std::vector<char> fifo;

  std::map<char, int> corrupted = {};
  const std::map<char, char> invers = {
      {'(', ')'}, {'<', '>'}, {'{', '}'}, {'[', ']'}};
  while (true) {
    std::getline(input, line);
    if (line.size() < 2)
      break;
    fifo.clear();
    for (auto c : line) {
      bool isBad = false;

      switch (c) {
      case '(':
      case '[':
      case '{':
      case '<':
        fifo.push_back(invers.at(c));
        break;
      case ')':
      case ']':
      case '}':
      case '>':
        if (fifo.empty()) {
          std::cout << "additional character?\n";
          isBad = true;
          break;
        }
        if (c != fifo.back()) {
          corrupted[c] += 1;
          isBad = true;
          break;
        }
        fifo.pop_back();
        break;
      default:
        std::cout << "Unknown character?\n";
      }
      if (isBad) {
        break;
      }
    }
  }

  std::cout << "Result "
            << corrupted[')'] * 3 + corrupted[']'] * 57 +
                   corrupted['}'] * 1197 + corrupted['>'] * 25137
            << "\n";
  return 0;
}
