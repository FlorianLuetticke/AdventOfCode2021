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

  void print(std::string separator = "") {
    for (auto y = 0; y < _row; ++y) {
      for (auto x = 0; x < _col; ++x) {
        if (value(x, y) == 0) {
          std::cout << "." << separator;
        } else if (value(x, y) > 9) {
          std::cout << "#" << separator;
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
};

struct Cost {
  size_t real;
  size_t estimate;
};

struct Amphi {
  bool moved = false;
  int x = 0;
  int y = 0;
  int desiredRoom = 1;
  int moveCost = 1;
  bool operator<(const Amphi &other) const {
    return moved < other.moved || (moved == other.moved && x < other.x) ||
           (moved == other.moved && x == other.x && y < other.y);
  }

  friend std::ostream &operator<<(std::ostream &stream, const Amphi &p) {
    stream << "Amphi{" << p.x << ", " << p.y << " target " << p.desiredRoom
           << ", moved " << p.moved << "} ";
    return stream;
  }
};

struct State {
  std::array<Amphi, 8> amphipods;

  static int stepsTo(Grid &g, int xStart, int yStart, int xEnd, int yEnd,
                     bool debug = false) {
    int steps = 0;
    if (debug)
      std::cout << " go from " << xStart << " " << yStart << " to " << xEnd
                << " " << yEnd << "\n";
    while (yStart > 0) {
      if (debug)
        std::cout << " Value at " << xStart << " " << yStart - 1 << " is "
                  << g.value(xStart, yStart - 1) << "\n";
      if (g.value(xStart, yStart - 1) == 0) {
        yStart--;
        ++steps;
      } else {
        return -1;
      }
    }
    while (xStart > xEnd) {
      if (debug)
        std::cout << " Value at " << xStart - 1 << " " << 0 << " is "
                  << g.value(xStart - 1, 0) << "\n";
      if (g.value(xStart - 1, 0) == 0) {
        xStart--;
        ++steps;
      } else {
        return -1;
      }
    }
    while (xStart < xEnd) {
      if (debug)
        std::cout << " Value at " << xStart + 1 << " " << 0 << " is "
                  << g.value(xStart + 1, 0) << "\n";
      if (g.value(xStart + 1, 0) == 0) {
        xStart++;
        ++steps;
      } else {
        return -1;
      }
    }

    while (yStart < yEnd) {
      if (debug)
        std::cout << " Value at " << xStart << " " << yStart + 1 << " is "
                  << g.value(xStart, yStart + 1) << "\n";
      if (g.value(xStart, yStart + 1) == 0) {
        yStart++;
        ++steps;
      } else {
        return -1;
      }
    }
    return steps;
  }

  std::vector<std::pair<State, size_t>>
  getNextPossibleStatesAndCost(size_t curCost) const {

    std::vector<std::pair<State, size_t>> ret;
    Grid g(12, 3);
    for (auto amphi : amphipods) {
      g.value(amphi.x, amphi.y) = amphi.desiredRoom + 1;
    }
    std::array<bool, 4> roomEntryAllowed{true, true, true, true};
    for (size_t i = 1; i <= 2; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        roomEntryAllowed[j] =
            roomEntryAllowed[j] &&
            (g.value(3 + 2 * j, i) == 0 || g.value(3 + 2 * j, i) == j + 1);
      }
    }
    // std::cout << "allowed" << roomEntryAllowed[0] << roomEntryAllowed[1]
    //           << roomEntryAllowed[2] << roomEntryAllowed[3] << "\n";

    for (auto i = 0; i < amphipods.size(); ++i) {
      auto &amphi = amphipods[i];

      if (amphi.moved && !roomEntryAllowed[amphi.desiredRoom]) {
        continue;
      }

      if (amphi.moved) {
        auto roomCoordinate = 3 + 2 * amphi.desiredRoom;
        if (amphi.y != 0) {
          continue;
        }
        // std::cout << "Try moving amphi  at " << amphi.x << " " << amphi.y
        //           << " with val " << g.value(amphi.x, amphi.y) << " to room "
        //           << roomCoordinate << "\n";
        if (int steps = stepsTo(g, amphi.x, amphi.y, roomCoordinate, 2);
            steps >= 0) {
          // std::cout << "Got something!\n";
          State s = *this;
          s.amphipods[i].x = roomCoordinate;
          s.amphipods[i].y = 2;
          ret.emplace_back(s, curCost + steps * amphi.moveCost);
        } else if (int steps = stepsTo(g, amphi.x, amphi.y, roomCoordinate, 1);
                   steps >= 0) {
          // std::cout << "Got something2!\n";
          State s = *this;
          s.amphipods[i].x = roomCoordinate;
          s.amphipods[i].y = 1;
          ret.emplace_back(s, curCost + steps * amphi.moveCost);
        }
      } else {
        for (auto xEnd : {1, 2, 4, 6, 8, 10, 11}) {
          if (int steps = stepsTo(g, amphi.x, amphi.y, xEnd, 0); steps >= 0) {
            State s = *this;
            s.amphipods[i].x = xEnd;
            s.amphipods[i].y = 0;
            s.amphipods[i].moved = true;
            ret.emplace_back(s, curCost + steps * amphi.moveCost);
          }
        }
      }
    }

    return ret;
  }

  // static size_t estimateRemainingCost(int curCol, int curRow, Grid &grid) {
  //   return size_t(std::abs(curCol - (grid._col - 1)) +
  //                 std::abs(curRow - (grid._row - 1)));
  // }

  bool finished() const {
    for (auto amphi : amphipods) {
      if (amphi.y == 0 || amphi.desiredRoom * 2 + 3 != amphi.x) {
        return false;
      }
    }
    return true;
  }
  bool operator<(const State &other) const {
    return amphipods < other.amphipods;
  }
  void print() {
    Grid g(13, 3);
    for (auto amphi : amphipods) {
      g.value(amphi.x, amphi.y) = amphi.desiredRoom + 1;
    }
    for (auto y = 1; y < g._row; ++y) {
      for (auto x = 0; x < g._col; ++x) {
        if (x != 3 && x != 5 && x != 7 && x != 9) {
          g.value(x, y) = 999;
        }
      }
    }
    g.value(0, 0) = 999;
    g.value(12, 0) = 999;
    g.print();
  }
};

bool operator<(const std::pair<State, size_t> &lhs,
               const std::pair<State, size_t> &rhs) {
  return lhs.second > rhs.second;
}

bool operator>(const std::pair<State, size_t> &lhs,
               const std::pair<State, size_t> &rhs) {
  return lhs.second < rhs.second;
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  Grid inGrid(input);
  Grid fieldCostGrid(5 * inGrid._col, 5 * inGrid._row);

  for (int subi = 0; subi < 5; ++subi)
    for (int subj = 0; subj < 5; ++subj)
      for (int i = 0; i < inGrid._col; ++i)
        for (int j = 0; j < inGrid._row; ++j) {
          fieldCostGrid.value(subi * inGrid._col + i, subj * inGrid._row + j) =
              1 + (inGrid.value(i, j) - 1 + subi + subj) % 9;
        }

  State start;
  // start.amphipods[0].desiredRoom = 0;
  // start.amphipods[0].x = 3;
  // start.amphipods[0].y = 2;
  // start.amphipods[1].desiredRoom = 0;
  // start.amphipods[1].x = 9;
  // start.amphipods[1].y = 2;
  // start.amphipods[2].desiredRoom = 1;
  // start.amphipods[2].x = 3;
  // start.amphipods[2].y = 1;
  // start.amphipods[2].moveCost = 10;
  // start.amphipods[3].desiredRoom = 1;
  // start.amphipods[3].x = 7;
  // start.amphipods[3].y = 1;
  // start.amphipods[3].moveCost = 10;
  // start.amphipods[4].desiredRoom = 2;
  // start.amphipods[4].x = 5;
  // start.amphipods[4].y = 1;
  // start.amphipods[4].moveCost = 100;
  // start.amphipods[5].desiredRoom = 2;
  // start.amphipods[5].x = 7;
  // start.amphipods[5].y = 2;
  // start.amphipods[5].moveCost = 100;
  // start.amphipods[6].desiredRoom = 3;
  // start.amphipods[6].x = 5;
  // start.amphipods[6].y = 2;
  // start.amphipods[6].moveCost = 1000;
  // start.amphipods[7].desiredRoom = 3;
  // start.amphipods[7].x = 9;
  // start.amphipods[7].y = 1;
  // start.amphipods[7].moveCost = 1000;

  start.amphipods[0].desiredRoom = 0;
  start.amphipods[0].x = 7;
  start.amphipods[0].y = 2;
  start.amphipods[1].desiredRoom = 0;
  start.amphipods[1].x = 9;
  start.amphipods[1].y = 1;
  start.amphipods[2].desiredRoom = 1;
  start.amphipods[2].x = 3;
  start.amphipods[2].y = 2;
  start.amphipods[2].moveCost = 10;
  start.amphipods[3].desiredRoom = 1;
  start.amphipods[3].x = 5;
  start.amphipods[3].y = 1;
  start.amphipods[3].moveCost = 10;
  start.amphipods[4].desiredRoom = 2;
  start.amphipods[4].x = 3;
  start.amphipods[4].y = 1;
  start.amphipods[4].moveCost = 100;
  start.amphipods[5].desiredRoom = 2;
  start.amphipods[5].x = 9;
  start.amphipods[5].y = 2;
  start.amphipods[5].moveCost = 100;
  start.amphipods[6].desiredRoom = 3;
  start.amphipods[6].x = 5;
  start.amphipods[6].y = 2;
  start.amphipods[6].moveCost = 1000;
  start.amphipods[7].desiredRoom = 3;
  start.amphipods[7].x = 7;
  start.amphipods[7].y = 1;
  start.amphipods[7].moveCost = 1000;

  std::priority_queue<std::pair<State, size_t>> queue;
  std::map<State, size_t> visited;
  visited[start] = 1;
  queue.push(std::make_pair(start, 0ul));
  start.print();
  size_t count = 0;
  while (!queue.empty()) {
    count++;
    auto &[state, cost] = queue.top();
    if (state.finished()) {
      break; // found target;
    }
    if (auto it = visited.find(state); it != visited.end()) {
      if (it->second <= cost) {
        queue.pop(); // found cheaper way here.
        continue;
      }
    }
    visited[state] = cost;

    // std::cout << "working on " << queue.top().first._curCol << " "
    //           << queue.top().first._curRow << " with cost "
    //           << queue.top().second.real << " " <<
    //           queue.top().second.estimate
    //           << " \n";
    auto newStates = state.getNextPossibleStatesAndCost(cost);
    queue.pop();
    for (auto &state : newStates) {
      // std::cout << "for cost " << state.second << "\n";
      // state.first.print();
      queue.push(state);
    }
  }

  std::cout << "worked on " << count << " fields\n";

  if (!queue.empty()) {
    std::cout << "Result " << queue.top().second << std::endl;
  } else {
    std::cout << "No result?\n";
  }
  return 0;
}
