#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <ostream>
#include <queue>
#include <stdexcept>
#include <unordered_map>
class Grid {
public:
  int _row = 0, _col = 0;
  std::vector<int> _data;
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
        switch (val) {
        case '.':
          _data.push_back(0);
          break;
        case '>':
          _data.push_back(1);
          break;
        case 'v':
          _data.push_back(2);
          break;
        default:
          throw std::logic_error("err");
        }
      }
      if (_data.size() - startIndex != size_t(_col)) {
        std::cout << " Error with size \n";
      }
    }
  }

  Grid(int col, int row) : _row(row), _col(col), _data(size_t(row * col), 0) {}

  int &value(int col, int row) {
    if (!inRange(col, row)) {
      std::cout << "invalid access" << col << " " << row << std::endl;
    }
    return _data.at(col + row * _col);
  }
  int &valueWraparound(int col, int row) {
    return _data.at(col % _col + (row % _row) * _col);
  }
  bool inRange(int col, int row) {
    return !(col < 0 || row < 0 || col >= _col || row >= _row);
  }

  void print(std::string separator = "") {
    for (auto y = 0; y < _row; ++y) {
      for (auto x = 0; x < _col; ++x) {
        if (value(x, y) == 0) {
          std::cout << "." << separator;
        } else if (value(x, y) == 1) {
          std::cout << ">" << separator;
        } else if (value(x, y) == 2) {
          std::cout << "v" << separator;
        } else {
          std::cout << std::setw(1) << value(x, y) << separator;
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
  bool operator==(const Grid other) {
    return _row == other._row && _col == other._col && _data == other._data;
  }
};

Grid advance(Grid &in) {
  Grid ret(in._col, in._row);

  for (auto y = 0; y < in._row; ++y) {
    for (auto x = 0; x < in._col; ++x) {
      if (in.value(x, y) == 1) {
        if (in.valueWraparound(x + 1, y) == 0) {
          ret.valueWraparound(x + 1, y) = 1;
        } else {
          ret.value(x, y) = 1;
        }
      }
    }
  }
  for (auto y = 0; y < in._row; ++y) {
    for (auto x = 0; x < in._col; ++x) {
      if (in.value(x, y) == 2) {
        if (in.valueWraparound(x, y + 1) != 2 &&
            ret.valueWraparound(x, y + 1) == 0) {
          ret.valueWraparound(x, y + 1) = 2;
        } else {
          ret.value(x, y) = 2;
        }
      }
    }
  }
  return ret;
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }

  Grid g(input);
  g.print();
  int steps = 0;
  while (true) {
    ++steps;
    auto next = advance(g);
    if (next == g) {
      break;
    }
    g = std::move(next);
  }
  std::cout << "final\n";
  g.print();

  std::cout << "Result " << steps << "\n";
  return 0;
}
