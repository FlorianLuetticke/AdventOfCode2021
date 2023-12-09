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

class Grid {
  size_t xMax, yMax;
  std::vector<int> data;
  size_t index(size_t x, size_t y) { return x * yMax + y; }

public:
  size_t xDim() { return xMax; }
  size_t yDim() { return yMax; }

  Grid(size_t x, size_t y) : xMax(x), yMax(y), data(x * y, 0) {}
  int &operator()(size_t x, size_t y) { return data[index(x, y)]; }
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
  auto begin() { return data.begin(); }
  auto end() { return data.end(); }

  void print() {
    for (auto x = 0ul; x < xMax; ++x) {
      for (auto y = 0ul; y < yMax; ++y) {
        std::cout << this->operator()(x, y) << ",";
      }
      std::cout << "\n";
    }
  }
};

class Line {
public:
  size_t x1;

  size_t y1;
  size_t x2;
  size_t y2;
  Line(size_t _x1, size_t _y1, size_t _x2, size_t _y2)
      : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

  void drawToGrid(Grid &grid, bool includeDiagonal) {
    if (!(grid.xDim() > std::max(x1, x2)) ||
        !(grid.yDim() > std::max(y1, y2))) {
      grid.resize_inplace(std::max(grid.xDim(), std::max(x1, x2) + 1),
                          std::max(grid.yDim(), std::max(y1, y2) + 1));
    }
    int xDir = x1 == x2 ? 0 : -1 + 2 * (x1 < x2);
    int yDir = y1 == y2 ? 0 : -1 + 2 * (y1 < y2);
    if (!includeDiagonal && xDir * yDir != 0)
      return;
    size_t curX = x1, curY = y1;
    ++grid(curX, curY);
    while (curX != x2 || curY != y2) {
      curX += xDir;
      curY += yDir;
      // std::cout << "write " << curX << " " << curY << "\n";
      ++grid(curX, curY);
    };
  }
};

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
  std::string pair1, pair2, sep;

  Grid grid(10, 10);
  while (input >> pair1 >> sep >> pair2) {
    auto [x1, y1] = csvToUIntPair(pair1);
    auto [x2, y2] = csvToUIntPair(pair2);

    Line line(x1, y1, x2, y2);
    std::cout << x1 << " " << y1 << " " << x2 << " " << y2 << "\n";

    line.drawToGrid(grid, true);
  }
  grid.print();
  auto result =
      std::count_if(grid.begin(), grid.end(), [](int val) { return val >= 2; });

  std::cout << "res " << result << "\n";
  return 0;
}
