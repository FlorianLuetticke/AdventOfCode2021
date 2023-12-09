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
  std::vector<size_t> incompleteScore;

  while (true) {
    std::getline(input, line);
    if (line.size() < 2)
      break;
    fifo.clear();
    bool isBad = false;

    for (auto c : line) {
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
      if (isBad)
        break;
    }
    if (!isBad && !fifo.empty()) {
      size_t score = 0;
      for (auto it = fifo.rbegin(); it != fifo.rend(); ++it) {
        score *= 5;
        switch (*it) {
        case '>':
          score += 1;
        case '}': // if its stupid
          score += 1;
        case ']': // and it works
          score += 1;
        case ')': // it isn't stupid
          score += 1;
          break;
        default:
          std::cout << "Unknown character2?\n";
        }
      }
      incompleteScore.push_back(score);
    }
  }
  std::nth_element(incompleteScore.begin(),
                   incompleteScore.begin() + incompleteScore.size() / 2,
                   incompleteScore.end());
  for (auto el : incompleteScore) {
    std::cout << el << " ";
  }
  std::cout << "\n";
  std::cout << incompleteScore.size() << "\n";
  std::cout << "Result " << incompleteScore[incompleteScore.size() / 2] << "\n";
  return 0;
}
