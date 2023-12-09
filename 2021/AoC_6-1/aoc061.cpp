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
const int daysToSimulate = 256;

std::array<size_t, daysToSimulate + 1> singleFishDevelopment() {
  std::array<size_t, daysToSimulate + 1> ret;
  std::vector<size_t> fishes;
  fishes.reserve(2000);
  fishes.push_back(0);
  ret[0] = 1;
  for (size_t i = 1; i <= daysToSimulate; ++i) {
    size_t newFishes = 0;
    for (auto &fish : fishes) {
      if (!fish) {
        fish = 6;
        newFishes++;
      } else {
        --fish;
      }
    }
    fishes.resize(fishes.size() + newFishes, 8);
    ret[i] = fishes.size();
  }
  for (auto val : ret) {
    std::cout << val << ", ";
  }
  std::cout << "\n";
  return ret;
}

std::array<size_t, daysToSimulate + 1> cleverSingleFishDevelopment() {
  std::array<size_t, daysToSimulate + 1> ret = {};
  std::array<size_t, 9> fishesPerAge = {};
  fishesPerAge.fill(0);
  fishesPerAge[0] = 1;
  ret[0] = 1;
  for (size_t i = 1; i <= daysToSimulate; ++i) {
    size_t swap = fishesPerAge[0];
    for (int j = 0; j < 8; ++j) {
      fishesPerAge[j] = fishesPerAge[j + 1];
    }
    fishesPerAge[6] += swap;
    fishesPerAge[8] = swap;
    ret[i] = std::accumulate(fishesPerAge.begin(), fishesPerAge.end(), 0ul);
  }
  for (auto val : ret) {
    std::cout << val << ", ";
  }
  std::cout << "\n";
  return ret;
}

std::vector<int> csvToIntVec(const std::string &input) {
  auto curPos = input.data();
  auto end = input.data() + input.size();
  std::vector<int> resultVec;
  while (curPos < end) {
    int result = 0;
    auto [ptr, ec] = std::from_chars(curPos, end, result);
    if (ec == std::errc()) {
      resultVec.push_back(result);
      if (*ptr == ',') {
        ++ptr;
      } else {
        return resultVec;
      }
      curPos = ptr;
    } else {
      throw std::system_error(std::make_error_code(ec));
    }
  }
  return resultVec;
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::string line;
  std::getline(input, line);
  std::vector<int> population = csvToIntVec(line);

  std::array<size_t, daysToSimulate + 1> single = cleverSingleFishDevelopment();
  size_t result = 0;
  for (auto fishAge : population) {
    result += single[daysToSimulate - fishAge];
    // result AFTER 80 days, our input is BEFORE Day 1. ->81 steps
  }

  std::cout << "res " << result << "\n";
  return 0;
}
