#include <algorithm>
#include <array>
#include <bitset>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <system_error>
#include <vector>
class Segment {
public:
  std::vector<std::string> uniqueValues;
  std::vector<std::string> testInput;
  std::map<std::string, size_t> lut;
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
  static bool firstIncludesSecond(const std::string &val,
                                  const std::string &subset) {
    return std::includes(val.begin(), val.end(), subset.begin(), subset.end());
  }
  size_t part2() {
    std::string oneString =
        *std::find_if(uniqueValues.begin(), uniqueValues.end(),
                      [](const auto &val) { return val.size() == 2; });
    std::string fourString =
        *std::find_if(uniqueValues.begin(), uniqueValues.end(),
                      [](const auto &val) { return val.size() == 4; });
    std::string sevenString =
        *std::find_if(uniqueValues.begin(), uniqueValues.end(),
                      [](const auto &val) { return val.size() == 3; });
    std::string eightString =
        *std::find_if(uniqueValues.begin(), uniqueValues.end(),
                      [](const auto &val) { return val.size() == 7; });

    lut[oneString] = 1;
    lut[fourString] = 4;
    lut[sevenString] = 7;
    lut[eightString] = 8;
    std::string diff;
    for (auto val : fourString) {
      if (oneString.find(val) == oneString.npos) {
        diff.push_back(val);
      }
    }
    for (auto &val : uniqueValues) {
      if (val.size() != 5 && val.size() != 6)
        continue;
      if (val.size() == 6) {
        if (firstIncludesSecond(val, oneString)) {
          if (firstIncludesSecond(val, fourString)) {
            lut[val] = 9;
          } else {
            lut[val] = 0;
          }
        } else {
          lut[val] = 6;
        }
      } else {
        if (firstIncludesSecond(val, oneString)) {
          lut[val] = 3;
        } else if (firstIncludesSecond(val, diff)) {
          lut[val] = 5;
        } else {
          lut[val] = 2;
        }
      }
    }

    size_t result = 0;
    for (auto val : testInput) {
      result = 10 * result + lut.at(val);
    }
    return result;
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
    result += segment.part2();
  }

  std::cout << "Result " << result << "\n";
  return 0;
}
