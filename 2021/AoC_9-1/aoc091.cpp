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

    std::cout << col << " " << row << " : " << value(col, row) << " <- "
              << ((col >= 1) ? value(col - 1, row) : -1) << " "
              << ((row >= 1) ? value(col, row - 1) : -1) << " "
              << ((col < _col - 1) ? value(col + 1, row) : -1) << " "
              << ((row < _row - 1) ? value(col, row + 1) : -1) << "\n";

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

  size_t part1() {
    size_t ret = 0;
    for (int j = 0; j < _row; ++j) {
      for (int i = 0; i < _col; ++i) {
        std::cout << value(i, j) << " ";
      }
      std::cout << std::endl;
    }
    auto loc = getBasinLocations();
    for (auto [i, j] : loc) {
      ret += value(i, j) + 1;
    }
    return ret;
  }
  size_t part2() { return 0; }
};

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }

  Grid grid(input);
  auto val = grid.part1();
  std::cout << "Result " << val << "\n";
  return 0;
}
