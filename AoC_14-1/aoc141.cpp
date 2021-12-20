#include <algorithm>
#include <array>
#include <bitset>
#include <cctype>
#include <charconv>
#include <cstddef>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <ostream>
#include <set>
#include <stdexcept>
#include <system_error>
#include <unordered_map>
#include <vector>
class Grid {
  size_t xMax, yMax;
  std::vector<int> data;
  size_t index(size_t x, size_t y) { return x * yMax + y; }

public:
  size_t xDim() { return xMax; }
  size_t yDim() { return yMax; }

  Grid(size_t x, size_t y) : xMax(x), yMax(y), data(x * y, 0) {}
  int &operator()(size_t x, size_t y) { return data.at(index(x, y)); }
  void resize_inplace(size_t xNew, size_t yNew) {
    std::cout << "resize to " << xNew << " " << yNew << "\n";
    Grid newGrid(xNew, yNew);
    for (auto x = 0ul; x < std::min(xNew, xMax); ++x) {
      for (auto y = 0ul; y < std::min(yNew, yMax); ++y) {
        newGrid(x, y) = this->operator()(x, y);
      }
    }
    data = std::move(newGrid.data);
    xMax = xNew;
    yMax = yNew;
  }

  void setAtAndResizeIfNeeded(size_t x, size_t y, int value) {
    if (x >= xMax || y >= yMax) {
      resize_inplace(std::max(x + 1, xMax), std::max(y + 1, yMax));
    }
    this->operator()(x, y) = value;
  }

  auto begin() { return data.begin(); }
  auto end() { return data.end(); }

  void print(std::string separator = ",") {
    for (auto y = 0ul; y < yMax; ++y) {
      for (auto x = 0ul; x < xMax; ++x) {
        std::cout << (this->operator()(x, y) == 1 ? "#" : ".") << separator;
      }
      std::cout << "\n";
    }
  }
};

template <typename T>
void printVec(const std::vector<T> &in, const std::string &sep = ",") {
  std::cout << "{";
  for (size_t i = 0; i < in.size(); ++i) {
    std::cout << in[i] << (i == in.size() - 1 ? "}" : sep);
  }
}

std::pair<size_t, size_t> csvToUIntPair(const std::string &in) {
  auto middle = in.find(",");
  std::pair<size_t, size_t> result;
  auto [_, ec] = std::from_chars(in.data(), in.data() + middle, result.first);
  auto [__, ec2] = std::from_chars(in.data() + middle + 1,
                                   in.data() + in.size(), result.second);
  if (ec != std::errc() || ec2 != std::errc()) {
    throw std::system_error(std::make_error_code(ec));
  }
  return result;
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::map<std::pair<char, char>, char> insertionRules;
  std::unordered_map<char, size_t> elemCount;
  std::list<char> elem;
  std::string in;
  std::getline(input, in);
  for (auto c : in) {
    elem.push_back(c);
    elemCount[c]++;
  }
  std::getline(input, in);
  while (true) {
    std::string in;
    std::getline(input, in);
    if (in.size() < 7)
      break;
    insertionRules[std::make_pair(in[0], in[1])] = in.at(6);
  }

  for (auto [pair, target] : insertionRules) {
    std::cout << pair.first << " and " << pair.second << " go to " << target
              << std::endl;
  }

  for (int i = 0; i < 10; ++i) {
    auto it1 = elem.begin();
    auto it2 = elem.begin();
    ++it2;
    while (it2 != elem.end()) {
      if (auto insertIt = insertionRules.find(std::make_pair(*it1, *it2));
          insertIt != insertionRules.end()) {
        elem.insert(it2, insertIt->second);
        elemCount[insertIt->second]++;
      }
      it1 = it2;
      it2++;
    }
    for (auto c : elem)
      std::cout << c;
    std::cout << std::endl;
  }
  size_t minElemCount = std::numeric_limits<size_t>::max();
  size_t maxElemCount = 0;

  for (auto [c, val] : elemCount) {
    std::cout << "value " << c << " is present " << val << std::endl;
    maxElemCount = std::max(val, maxElemCount);
    minElemCount = std::min(val, minElemCount);
  }

  std::cout << "Result " << maxElemCount - minElemCount << std::endl;
  return 0;
}
