#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
// 1 wins on same amount
int findMostCommonNthBit(const std::vector<std::string> input, size_t n) {
  int nOnes = 0;
  for (const auto &val : input)
    nOnes += -1 + 2 * (val[n] == '1');
  return nOnes >= 0;
}

size_t intFromBinaryString(const std::string &val) {
  return std::accumulate(val.begin(), val.end(), int(0),
                         [](int res, char c) { return 2 * res + (c == '1'); });
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::vector<std::string> data;
  std::copy(std::istream_iterator<std::string>(input),
            std::istream_iterator<std::string>(), std::back_inserter(data));

  std::vector<std::string> filtered = data;
  int n = 0;
  while (filtered.size() > 1) {
    char filterFor = findMostCommonNthBit(filtered, n) ? '1' : '0';
    filtered.erase(std::remove_if(filtered.begin(), filtered.end(),
                                  [n, filterFor](const std::string &val) {
                                    return val[n] != filterFor;
                                  }),
                   filtered.end());
    ++n;
  }
  auto val1 = filtered.at(0);
  filtered = data;
  n = 0;
  while (filtered.size() > 1) {
    char filterFor = findMostCommonNthBit(filtered, n) ? '0' : '1';
    filtered.erase(std::remove_if(filtered.begin(), filtered.end(),
                                  [n, filterFor](const std::string &val) {
                                    return val[n] != filterFor;
                                  }),
                   filtered.end());
    ++n;
  }
  auto val2 = filtered.at(0);

  std::cout << val1 << " " << val2 << " -> " << intFromBinaryString(val1) << " "
            << intFromBinaryString(val2) << "\n";

  std::cout << "Result " << std::dec
            << intFromBinaryString(val1) * intFromBinaryString(val2) << "\n";
  return 0;
}
