#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <ostream>
#include <queue>
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

  Grid(int col, int row) : _row(row), _col(col), _data(size_t(row * col), 0) {}

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

struct Cost {
  size_t real;
  size_t estimate;
};

struct SubState {

  int _curCol = 0, _curRow = 0;
  static constexpr std::array<std::pair<int, int>, 4> offsets = {
      {{-1, 0}, {0, -1}, {0, 1}, {1, 0}}};

  SubState(int curCol, int curRow) : _curCol(curCol), _curRow(curRow) {}

  std::vector<std::pair<SubState, Cost>>
  getNextPossibleStatesAndCost(Grid &grid, Grid &visited,
                               size_t curCost) const {
    std::vector<std::pair<SubState, Cost>> ret;
    ret.reserve(4);
    for (auto [i, j] : offsets) {
      if (grid.inRange(_curCol + i, _curRow + j)) {
        auto newCost = curCost + size_t(grid.value(_curCol + i, _curRow + j));
        if (visited.value(_curCol + i, _curRow + j) == 0 ||
            (visited.value(_curCol + i, _curRow + j) > newCost)) {
          ret.emplace_back(
              SubState(_curCol + i, _curRow + j),
              Cost{newCost, newCost + estimateRemainingCost(
                                          _curCol + i, _curRow + j, grid)});
        }
      }
    }
    return ret;
  }

  static size_t estimateRemainingCost(int curCol, int curRow, Grid &grid) {
    return size_t(std::abs(curCol - (grid._col - 1)) +
                  std::abs(curRow - (grid._row - 1)));
  }

  bool finished(Grid &grid) const {
    return _curCol == (grid._col - 1) && _curRow == (grid._row - 1);
  }
};

bool operator<(const std::pair<SubState, Cost> &lhs,
               const std::pair<SubState, Cost> &rhs) {
  return lhs.second.estimate > rhs.second.estimate;
}

bool operator>(const std::pair<SubState, Cost> &lhs,
               const std::pair<SubState, Cost> &rhs) {
  return lhs.second.estimate < rhs.second.estimate;
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  Grid fieldCostGrid(input);

  Grid visited(fieldCostGrid._col, fieldCostGrid._row);
  visited.value(0, 0) = 1;

  std::priority_queue<std::pair<SubState, Cost>> queue;

  queue.push(std::make_pair(
      SubState(0, 0),
      Cost{0uL, SubState::estimateRemainingCost(0, 0, fieldCostGrid)}));

  size_t count = 0;
  while (!queue.empty()) {
    count++;
    if (queue.top().first.finished(fieldCostGrid)) {
      break; // found target;
    }
    if (queue.top().second.real >
        visited.value(queue.top().first._curCol, queue.top().first._curRow)) {
      queue.pop(); // found cheaper way here.
      continue;
    }

    // std::cout << "working on " << queue.top().first._curCol << " "
    //           << queue.top().first._curRow << " with cost "
    //           << queue.top().second.real << " " <<
    //           queue.top().second.estimate
    //           << " \n";
    auto curCost = queue.top().second.real;
    auto newStates = queue.top().first.getNextPossibleStatesAndCost(
        fieldCostGrid, visited, curCost);
    queue.pop();
    for (auto &state : newStates) {

      visited.value(state.first._curCol, state.first._curRow) =
          state.second.real;
      queue.push(state);
    }
  }
  // fieldCostGrid.print("");
  // std::cout << " cost till here\n";
  // visited.print("");
  std::cout << "worked on " << count << " fields\n";

  if (!queue.empty()) {
    std::cout << "Result " << queue.top().second.real << std::endl;
  } else {
    std::cout << "No result?\n";
  }
  return 0;
}
