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
#include <map>
#include <numeric>
#include <set>
#include <stdexcept>
#include <system_error>
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

  Grid grid(10, 10);

  std::vector<std::pair<bool, size_t>> folds;
  std::string in;
  while (true) {
    std::string in;
    std::getline(input, in);
    if (in.size() < 2)
      break;
    auto [x, y] = csvToUIntPair(in);
    grid.setAtAndResizeIfNeeded(x, y, 1);
  }
  while (true) {
    std::string in;
    std::getline(input, in);
    if (in.size() < 2)
      break;
    std::cout << ">>" << in << "<< " << in[11] << "\n";
    size_t result;
    auto [ptr, ec] =
        std::from_chars(in.data() + 13, in.data() + in.size(), result);
    if (ec != std::errc()) {
      throw std::system_error(std::make_error_code(ec));
    }
    if (in[11] != 'x' && in[11] != 'y') {
      throw std::logic_error("strange");
    }
    folds.emplace_back(in[11] == 'x', result);
  }
  grid.print();
  for (auto fold : folds) {
    std::cout << "folding along " << (fold.first ? "x" : "y") << " at "
              << fold.second << "\n";
    if (fold.first) {
      for (size_t x = fold.second + 1; x < grid.xDim(); ++x) {
        for (size_t y = 0; y < grid.yDim(); ++y) {
          if (grid(x, y) == 1) {
            grid(x, y) = 0;
            grid(fold.second - (x - fold.second), y) = 1;
          }
        }
      }
      grid.resize_inplace(fold.second, grid.yDim());
    } else {
      for (size_t x = 0; x < grid.xDim(); ++x) {
        for (size_t y = fold.second + 1; y < grid.yDim(); ++y) {
          if (grid(x, y) == 1) {
            grid(x, y) = 0;
            grid(x, fold.second - (y - fold.second)) = 1;
          }
        }
      }
      grid.resize_inplace(grid.xDim(), fold.second);
    }
    grid.print("");
  }

  std::cout << "Result " << std::count(grid.begin(), grid.end(), 1)
            << std::endl;
  return 0;
}
