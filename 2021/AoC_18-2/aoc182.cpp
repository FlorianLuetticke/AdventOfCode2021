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
#include <memory>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
struct ModifyParent {
  bool leftChanges = false;
  unsigned leftValue = 0;
  bool rightChanges = false;
  unsigned rightValue = 0;
};

class SnailfishNumber {

  unsigned number;
  std::shared_ptr<SnailfishNumber> lhs;
  std::shared_ptr<SnailfishNumber> rhs;

public:
  void print() {
    if (lhs == nullptr) {
      std::cout << number;
      return;
    }
    std::cout << "[";
    lhs->print();
    std::cout << ",";
    rhs->print();
    std::cout << "]";
  }

  SnailfishNumber(std::string::const_iterator &in, unsigned dep) {
    unsigned curDepth = 0;
    char val = *in;

    if (val >= '0' && val <= '9') {
      number = unsigned(val - '0');
      in++;
      return;
    }

    if (*in != '[')
      throw std::logic_error("Strange stuff in string");
    in++;
    lhs = std::make_shared<SnailfishNumber>(in, dep + 1);
    if (*in != ',')
      throw std::logic_error("Strange stuff in string");
    in++;
    rhs = std::make_shared<SnailfishNumber>(in, dep + 1);

    if (*in != ']')
      throw std::logic_error("Strange stuff in string");
    in++;
  }

  SnailfishNumber(const SnailfishNumber &first, const SnailfishNumber &second) {
    lhs = std::make_shared<SnailfishNumber>(first);
    rhs = std::make_shared<SnailfishNumber>(second);
  }

  SnailfishNumber(unsigned value) { number = value; }

  bool split() {
    if (lhs == nullptr) {
      if (number < 10)
        return false;
      unsigned newNumber = number / 2 + (number % 2);
      lhs = std::make_shared<SnailfishNumber>(newNumber);
      rhs = std::make_shared<SnailfishNumber>(number / 2);
      number = 0;
      return true;
    }
    if (lhs->split()) {
      return true;
    }
    return rhs->split();
  }

  void addToLeftmost(unsigned val) {
    if (lhs == nullptr) {
      number += val;
    } else {
      lhs->addToLeftmost(val);
    }
  }

  void addToRightmost(unsigned val) {
    if (lhs == nullptr) {
      number += val;
    } else {
      rhs->addToLeftmost(val);
    }
  }
  bool explode() {
    auto m = recurse_explode(0);
    return m.has_value();
  }

  std::optional<ModifyParent> recurse_explode(unsigned depth) {
    if (lhs == nullptr) {
      return std::nullopt;
    } else {
      if (lhs->lhs == nullptr && depth > 3) {
        ModifyParent m;
        m.leftChanges = true;
        m.leftValue = lhs->number;
        m.rightChanges = true;
        m.rightValue = rhs->number;
        lhs = nullptr;
        rhs = nullptr;
        number = 0;
        return m;
      }
      auto m = lhs->recurse_explode(depth + 1);
      if (m) {
        if (rhs != nullptr) {
          if (m->rightChanges) {
            m->rightChanges = false;
            rhs->addToLeftmost(m->rightValue);
          }
        }
        return m;
      }
      m = rhs->recurse_explode(depth + 1);
      if (m) {
        if (lhs != nullptr) {
          if (m->leftChanges) {
            m->leftChanges = false;
            rhs->addToRightmost(m->leftValue);
          }
        }
        return m;
      }
    }
    return std::nullopt;
  }

  void reduce() {
    bool hasChanged = true;
    do {
      print();
      std::cout << "\n";
      hasChanged = explode();
      if (hasChanged)
        continue;
      hasChanged = split();
      if (hasChanged)
        continue;
    } while (hasChanged);
  }
};

// SnailfishNumber &operator+=(const SnailfishNumber &other) {
//   _data.insert(_data.end(), other._data.begin(), other._data.end());
//   for (auto &pos : _data) {
//     pos.depth += 1;
//   }
//   reduce();
//   return *this;
// }
// bool explodeOne() {
//   auto it = _data.begin();
//   for (auto it = _data.begin(); it != _data.end(); ++it) {
//     if (it->depth > 4) {
//       if (it != _data.begin()) {
//         auto prev = it;
//         prev--;
//         prev->number += it->number;
//       }
//       it->number = 0;
//       it->depth -= 1;
//       ++it;
//       if (it == _data.end()) {
//         std::cout << "this is an error\n";
//         exit(-1);
//       }
//       auto next = it;
//       next++;
//       if (next != _data.end()) {
//         next->number += it->number;
//       }
//       _data.erase(it);
//       return true;
//     }
//   }
//   return false;
// }

// bool splitOne() {
//   auto it = _data.begin();
//   for (auto it = _data.begin(); it != _data.end(); ++it) {
//     if (it->number >= 10) {
//       unsigned newNumber = it->number / 2 + (it->number % 2);
//       it->number = it->number / 2;
//       it->depth += 1;
//       unsigned newDepth = it->depth;
//       ++it;
//       _data.insert(it, position{newNumber, newDepth});
//       return true;
//     }
//   }
//   return false;
// }
// void print() const { printList(_data); }
// void reduce() {
//   bool hasChanged = true;
//   do {
//     print();
//     hasChanged = explodeOne();
//     if (hasChanged)
//       continue;
//     hasChanged = splitOne();
//     if (hasChanged)
//       continue;
//   } while (hasChanged);
// }

// unsigned magnitude() {
//   auto copy = _data;
//   for (unsigned depth = 4; depth > 0; --depth) {
//     printList(copy);
//     for (auto it = copy.begin(); it != copy.end(); ++it) {
//       if (it->depth > depth) {
//         std::cout << "magnitude depth error\n";
//       }
//       if (it->depth >= depth) {
//         auto next = it;
//         ++next;
//         it->number = 3 * it->number + 2 * next->number;
//         it->depth -= 1;
//         copy.erase(next);
//       }
//     }
//   }
//   printList(copy);
//   return copy.front().number;
// }
// }
// ;

// SnailfishNumber operator+(const SnailfishNumber &lhs,
//                           const SnailfishNumber rhs) {
//   SnailfishNumber ret = lhs;
//   ret += rhs;
//   return ret;
// }
SnailfishNumber operator+(const SnailfishNumber &first,
                          const SnailfishNumber &second) {
  return SnailfishNumber(first, second);
}

int main(int argc, char **argv) {
  std::ifstream input("inputTest.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }

  std::vector<SnailfishNumber> numbers;
  while (true) {
    std::string line;
    std::getline(input, line);
    if (line.size() < 2)
      break;
    auto it = line.cbegin();
    auto sfn = SnailfishNumber(it, 1);
    std::cout << ">>" << line << ">>\n";
    sfn.print();
    numbers.push_back(sfn);
    std::cout << "\n";
  }
  auto sfn = numbers.at(0) + numbers.at(1);
  sfn.print();
  std::cout << "\n";
  sfn.reduce();
  sfn.print();
  std::cout << "\n";
  unsigned max = 0;

  std::cout << "Result " << max << "\n";

  return 0;
}
