#include <algorithm>
#include <array>
#include <bitset>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <system_error>
#include <vector>

class Segment {
public:
  std::vector<std::string> uniqueValues;
  std::vector<std::string> testInput;
  Segment(std::ifstream &input) {
    std::vector<std::string> inTokens(15);
    for (auto &val : inTokens) {
      if (!(input >> val)) {
        throw std::out_of_range("not enougth data");
      }
      std::sort(val.begin(), val.end());
    }
    if (inTokens[10] != "|")
      throw std::out_of_range("bad data");

    uniqueValues.assign(inTokens.begin(), inTokens.begin() + 10);
    testInput.assign(inTokens.begin() + 11, inTokens.begin() + 15);
  }

  size_t part1() {
    return std::count_if(testInput.begin(), testInput.end(),
                         [](const auto &val) {
                           return val.size() == 2 || val.size() == 3 ||
                                  val.size() == 4 || val.size() == 7;
                         });
  }
};

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }

  std::vector<Segment> segments;
  while (true) {
    try {
      segments.emplace_back(input);
    } catch (std::out_of_range &e) {
      std::cout << "read " << std::dec << segments.size() << " segments\n";
      break;
    }
  }
  size_t result = 0;
  for (auto &segment : segments) {
    result += segment.part1();
  }

  std::cout << "Result " << result << "\n";
  return 0;
}
