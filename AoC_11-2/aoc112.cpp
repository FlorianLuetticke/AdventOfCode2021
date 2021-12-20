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
  static constexpr std::array<std::pair<int, int>, 8> offsets = {
      {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

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
  int &value(int col, int row) {
    if (!inRange(col, row)) {
      std::cout << "invalid access" << col << " " << row << std::endl;
    }
    return _data.at(col + row * _col);
  }
  bool inRange(int col, int row) {
    return !(col < 0 || row < 0 || col >= _col || row >= _row);
  }

  void print(std::string separator = ",") {
    for (auto y = 0; y < _row; ++y) {
      for (auto x = 0; x < _col; ++x) {
        std::cout << value(x, y) << separator;
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
  size_t advanceAndCountFlashes() {

    for (int i = 0; i < _col; ++i) {
      for (int j = 0; j < _row; ++j) {
        ++value(i, j);
      }
    }
    size_t flashes = 0;
    for (int i = 0; i < _col; ++i) {
      for (int j = 0; j < _row; ++j) {
        if (value(i, j) > 9) {
          value(i, j) = 0;
          flashes += 1 + recurseNeighboursAndCount(i, j);
        }
      }
    }
    return flashes;
  }
  size_t recurseNeighboursAndCount(int col, int row) {
    for (auto [cOff, rOff] : offsets) {
      if (inRange(col + cOff, row + rOff) &&
          value(col + cOff, row + rOff) > 0) {
        ++value(col + cOff, row + rOff);
      }
    }
    size_t flashes = 0;
    for (auto [cOff, rOff] : offsets) {
      if (inRange(col + cOff, row + rOff) &&
          value(col + cOff, row + rOff) > 9) {
        value(col + cOff, row + rOff) = 0;
        flashes += 1 + recurseNeighboursAndCount(col + cOff, row + rOff);
      }
    }
    return flashes;
  }
};

int main(int argc, char **argv) {
  std::ifstream input("inputTest2.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  Grid grid(input);
  auto step = 0;
  while (true) {
    step++;
    auto flashes = grid.advanceAndCountFlashes();
    std::cout << " Step " << step << "\n";
    grid.print();
    if (step > 20000) {
      std::cout << " no end so far";
      break;
    }
    if (flashes == grid._row * grid._col) {
      break;
    }
  }

  std::cout << "Result " << step << "\n";
  return 0;
}
