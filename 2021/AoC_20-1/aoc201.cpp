#include <algorithm>
#include <array>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <optional>
#include <ostream>
#include <set>
#include <stdexcept>
#include <type_traits>
class Grid {
public:
  int _row = 0, _col = 0;
  std::vector<int> _data;
  Grid(std::ifstream &input, int extend = 0) {
    while (true) {
      std::string line;
      std::getline(input, line);
      if (line.size() < 5)
        break;
      if (_col == 0) {
        _col = line.size() + 2 * extend;
        _row += extend;
        for (auto i = 0; i < extend * _col; ++i) {
          _data.push_back(0);
        }
      }

      _row += 1;
      size_t startIndex = _data.size();

      for (auto i = 0; i < extend; ++i) {
        _data.push_back(0);
      }
      for (auto val : line) {
        _data.push_back(val == '#');
      }
      for (auto i = 0; i < extend; ++i) {
        _data.push_back(0);
      }
      if (_data.size() - startIndex != size_t(_col)) {
        std::cout << " Error with size \n";
      }
    }
    _row += extend;
    for (auto i = 0; i < extend * _col; ++i) {
      _data.push_back(0);
    }
  }

  Grid(int col, int row) : _row(row), _col(col), _data(size_t(row * col), 0) {}

  int &value(int col, int row) {
    if (!inRange(col, row)) {
      std::cout << "invalid access" << col << " " << row << std::endl;
    }
    return _data.at(col + row * _col);
  }
  int getExtended(int col, int row, int extend = 0) const {
    if (!inRange(col, row)) {
      return extend;
    }
    return _data.at(col + row * _col);
  }
  bool inRange(int col, int row) const {
    return !(col < 0 || row < 0 || col >= _col || row >= _row);
  }
  auto begin() { return _data.begin(); }
  auto end() { return _data.end(); }

  void printHash(std::string separator = "") {
    for (auto y = 0; y < _row; ++y) {
      for (auto x = 0; x < _col; ++x) {
        if (value(x, y) == 0) {
          std::cout << "." << separator;
        } else {
          std::cout << "#" << separator;
        }
      }
      std::cout << "\n";
    }
  }
  void print(std::string separator = ",") {
    for (auto y = 0; y < _row; ++y) {
      for (auto x = 0; x < _col; ++x) {
        if (value(x, y) == 0) {
          std::cout << " . " << separator;
        } else if (value(x, y) > 999) {
          std::cout << "###" << separator;
        } else {
          std::cout << std::setw(3) << value(x, y) << separator;
        }
      }
      std::cout << "\n";
    }
  }

  bool operator<(const Grid other) {
    return _row < other._row || (_row == other._row && _col < other._col) ||
           (_row == other._row && _col == other._col &&
            std::lexicographical_compare(_data.begin(), _data.end(),
                                         other._data.begin(),
                                         other._data.end()));
  }
};

Grid enhance(const Grid &grid, Grid &lut, int extend) {
  Grid output(grid._col, grid._row);
  for (int i = 0; i < grid._col; ++i) {
    for (int j = 0; j < grid._row; ++j) {
      int val = 0;
      for (int sj = -1; sj <= 1; ++sj) {
        for (int si = -1; si <= 1; ++si) {
          val = 2 * val + grid.getExtended(i + si, j + sj, extend);
        }
      }
      output.value(i, j) = lut.value(val, 0);
    }
  }
  return output;
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  Grid lut(input);
  Grid grid(input, 52);
  if (lut._row != 1 || lut._col != 512) {
    throw std::logic_error("bad input?");
  }
  long solution1 = 0;
  for (int i = 0; i < 25; ++i) {
    grid = enhance(grid, lut, 0);
    grid = enhance(grid, lut, lut.value(0, 0));
    std::cout << "Enhance " << 2 * i + 2 << "\n";
    grid.printHash();
    if (i == 0) {
      solution1 = std::accumulate(grid.begin(), grid.end(), 0);
    }
  }

  // for (int i = 0; i < grid._col; ++i) {
  //   for (int j = 0; j < grid._row; ++j) {
  //     if (grid.value(i, j) != other.value(i, j)) {
  //       std::cout << "difference at " << i << " " << j << " -> "
  //                 << grid.value(i, j) << " vs " << other.value(i, j)
  //                 << std::endl;
  //     }
  //   }
  // }
  std::cout << "Result 1 " << solution1 << "\n";
  std::cout << "Result 2 " << std::accumulate(grid.begin(), grid.end(), 0)
            << "\n";
  return 0;
}
