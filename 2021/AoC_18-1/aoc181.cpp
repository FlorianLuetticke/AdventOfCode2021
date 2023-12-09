#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>

class SnailfishNumber {
  struct position {
    unsigned number;
    unsigned depth;
  };
  std::list<position> _data;
  static void printList(const std::list<position> &data) {
    unsigned lastDepth = 0;
    // for (auto &pos : data) {
    //   std::cout << pos.depth << " " << pos.number << "\n";
    // }
    for (auto &pos : data) {
      if (lastDepth < pos.depth) {
        for (auto d = lastDepth; d < pos.depth; ++d)
          std::cout << "[";
      } else if (lastDepth > pos.depth) {
        for (auto d = lastDepth; d > pos.depth; --d)
          std::cout << "]";
        std::cout << ",";
      } else {
        std::cout << ",";
      }
      lastDepth = pos.depth;
      std::cout << pos.number;
    }
    for (auto d = lastDepth; d != 0; --d)
      std::cout << "]";
    std::cout << "\n";
  }

public:
  SnailfishNumber &operator+=(const SnailfishNumber &other) {
    _data.insert(_data.end(), other._data.begin(), other._data.end());
    for (auto &pos : _data) {
      pos.depth += 1;
    }
    return *this;
  }
  bool explodeOne() {
    auto it = _data.begin();
    for (auto it = _data.begin(); it != _data.end(); ++it) {
      if (it->depth > 4) {
        if (it != _data.begin()) {
          auto prev = it;
          prev--;
          prev->number += it->number;
        }
        it->number = 0;
        it->depth -= 1;
        ++it;
        if (it == _data.end()) {
          std::cout << "this is an error\n";
          exit(-1);
        }
        auto next = it;
        next++;
        if (next != _data.end()) {
          next->number += it->number;
        }
        _data.erase(it);
        return true;
      }
    }
    return false;
  }

  bool splitOne() {
    auto it = _data.begin();
    for (auto it = _data.begin(); it != _data.end(); ++it) {
      if (it->number >= 10) {
        unsigned newNumber = it->number / 2 + (it->number % 2);
        it->number = it->number / 2;
        it->depth += 1;
        unsigned newDepth = it->depth;
        ++it;
        _data.insert(it, position{newNumber, newDepth});
        return true;
      }
    }
    return false;
  }
  void print() const { printList(_data); }
  void reduce() {
    bool hasChanged = true;
    do {
      print();
      hasChanged = explodeOne();
      if (hasChanged)
        continue;
      hasChanged = splitOne();
      if (hasChanged)
        continue;
    } while (hasChanged);
  }

  SnailfishNumber(const std::string &in) {
    unsigned curDepth = 0;
    for (size_t i = 0; i < in.size(); ++i) {
      if (in[i] >= '0' && in[i] <= '9') {
        _data.emplace_back(position{unsigned(in[i] - '0'), curDepth});
      } else {
        switch (in[i]) {
        case '[':
          curDepth += 1;
          break;
        case ']':
          curDepth -= 1;
          break;
        case ',':
          break;
        default:
          std::cout << "unknown input " << in[i] << " found \n";
          exit(-1);
        }
      }
    }
  }

  unsigned magnitude() {
    auto copy = _data;
    for (unsigned depth = 4; depth > 0; --depth) {
      printList(copy);
      for (auto it = copy.begin(); it != copy.end(); ++it) {
        if (it->depth > depth) {
          std::cout << "magnitude depth error\n";
        }
        if (it->depth >= depth) {
          auto next = it;
          ++next;
          it->number = 3 * it->number + 2 * next->number;
          it->depth -= 1;
          copy.erase(next);
        }
      }
    }
    printList(copy);
    return copy.front().number;
  }
};

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }

  std::string line;
  std::getline(input, line);

  SnailfishNumber sum(line);
  std::cout << "start number\n";
  sum.print();
  while (true) {
    std::string line;
    std::getline(input, line);
    if (line.size() < 2)
      break;
    SnailfishNumber add(line);
    std::cout << "add number\n";
    add.print();
    sum += add;
    std::cout << "result\n";
    sum.print();
    sum.reduce();
    std::cout << "result\n";
    sum.print();
  }
  std::cout << "Calculate Magnitude\n";
  auto result = sum.magnitude();

  std::cout << "Result " << result << "\n";

  return 0;
}
