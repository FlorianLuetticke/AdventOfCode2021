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
class Grid {
public:
  std::vector<int> _data;
  int _row = 0, _col = 0;
  Grid(std::ifstream &input) {
    while (true) {
      std::string line;
      std::getline(input, line);
      if (line.size() < 5)
        break;
      if (_col == 0)
        _col = line.size();

      _row += 1;
      size_t startIndex = _data.size();
      for (auto val : line) {
        int number = (val - '0');
        if (number >= 0 && number < 10) {
          _data.push_back(number);
        }
      }
      if (_data.size() - startIndex != size_t(_col)) {
        std::cout << " Error with size \n";
      }
    }
  }
  int &value(int col, int row) { return _data[col + row * _col]; }

  bool neighboursBigger(int col, int row) {
    if (col >= 1 && value(col, row) >= value(col - 1, row)) {
      return false;
    }
    if (row >= 1 && value(col, row) >= value(col, row - 1)) {
      return false;
    }
    if (col < _col - 1 && value(col, row) >= value(col + 1, row)) {
      return false;
    }
    if (row < _row - 1 && value(col, row) >= value(col, row + 1)) {
      return false;
    }

    return true;
  }
  std::vector<std::pair<int, int>> getBasinLocations() {
    std::vector<std::pair<int, int>> ret;
    for (int i = 0; i < _col; ++i) {
      for (int j = 0; j < _row; ++j) {
        if (neighboursBigger(i, j)) {
          ret.emplace_back(i, j);
        }
      }
    }
    return ret;
  }
};

size_t recursiveSearch(Grid &grid, int col, int row) {
  if (col < 0 || row < 0 || col >= grid._col || row >= grid._row ||
      grid.value(col, row) == 9) {
    return 0;
  }

  grid.value(col, row) = 9;
  return 1 + recursiveSearch(grid, col - 1, row) +
         recursiveSearch(grid, col, row - 1) +
         recursiveSearch(grid, col + 1, row) +
         recursiveSearch(grid, col, row + 1);
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }

  Grid grid(input);
  auto basins = grid.getBasinLocations();
  std::vector<size_t> sizes;
  for (auto [i, j] : basins) {
    sizes.push_back(recursiveSearch(grid, i, j));
    if (sizes.back() == 0) {
      std::cout << "basin with size 0 was overwritten\n";
    }
  }

  std::nth_element(sizes.begin(), sizes.end() - 3, sizes.end());

  auto val = std::accumulate(sizes.end() - 3, sizes.end(), 1,
                             std::multiplies<size_t>());
  std::cout << "Result " << val << "\n";
  return 0;
}
