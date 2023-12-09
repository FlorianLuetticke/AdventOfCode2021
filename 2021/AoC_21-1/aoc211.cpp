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
#include <utility>
class Dice {
public:
  int state = 1;
  // {6,5,4,3,2,1,0,9,8,7}
  int get() {
    if (state <= 98) {
      int ret = 3 * state + 3;
      state += 3;
      if (state > 100)
        state -= 100;
      return ret;
    } else {
      int ret = 0;
      for (int i = 0; i < 3; ++i) {
        ret += state;
        ++state;
        if (state == 101)
          state = 1;
      }
      return ret;
    }
  }
};

void part1() {
  int pos1 = 8, pos2 = 7;
  int score1 = 0, score2 = 0;
  Dice dice;
  int diceThrows = 0;
  while (score1 < 1000 && score2 < 1000) {
    auto diceVal = dice.get();
    pos1 = 1 + (pos1 - 1 + diceVal) % 10;
    std::cout << "pos 1 now at " << pos2 << " with score " << score2
              << " after throw " << diceVal << "\n";
    score1 += pos1;
    diceThrows += 3;
    if (score1 >= 1000)
      break;
    diceVal = dice.get();
    pos2 = 1 + (pos2 - 1 + diceVal) % 10;
    score2 += pos2;
    std::cout << "pos 2 now at " << pos2 << " with score " << score2
              << " after throw " << diceVal << "\n";
    diceThrows += 3;
  }

  std::cout << "Result 1 " << diceThrows * std::min(score1, score2) << "\n";
}

long advance(std::vector<std::vector<long>> &in) {
  long wins = 0;
  std::vector<std::vector<long>> out(10, std::vector<long>(21, 0));

  for (size_t field = 0; field < 10; ++field) {
    for (size_t fieldOffset = 1; fieldOffset <= 3; ++fieldOffset) {
      long newField = (field + fieldOffset) % 10;
      for (size_t score = 0; score < 21; ++score) {
        long newScore = score + 1 + newField;

        if (newScore >= 21) {
          wins += in[field][score];
        } else {
          out.at(newField).at(newScore) += in.at(field).at(score);
        }
      }
    }
  }
  in = out;
  return wins;
}

void print(std::vector<std::vector<long>> data, std::string separator = ",") {
  for (auto y = 0; y < data.size(); ++y) {
    for (auto x = 0; x < data[y].size(); ++x) {
      if (data[y][x] == 0) {
        std::cout << " . " << separator;
      } else if (data[y][x] > 999) {
        std::cout << "###" << separator;
      } else {
        std::cout << std::setw(3) << data.at(y).at(x) << separator;
      }
    }
    std::cout << "\n";
  }
}
std::vector<std::pair<long, long>> diceVals = {{3, 1}, {4, 3}, {5, 6}, {6, 7},
                                               {7, 6}, {8, 3}, {9, 1}};
std::pair<long, long> doStep(int currentPos, int currentScore, int otherPos,
                             int otherScore, int depth) {
  if (otherScore >= 21) {
    return {0, 1};
  }
  auto ret = std::make_pair(0l, 0l);
  for (auto [val, freq] : diceVals) {

    auto newPos = 1 + (currentPos - 1 + val) % 10;
    auto newScore = currentScore + newPos;
    auto [first, second] =
        doStep(otherPos, otherScore, newPos, newScore, depth + 1);
    // if (first != 100 && second != 100) {
    //   for (int i = 0; i < depth; ++i)
    //     std::cout << "   ";
    //   std::cout << currentPos << " " << currentScore << " " << otherPos << "
    //   "
    //             << otherScore << "\n";
    //   for (int i = 0; i < depth; ++i)
    //     std::cout << "   ";
    //   std::cout << "New games " << first << " " << second << " multiply" <<
    //   freq
    //             << "\n";
    // }
    ret.first += freq * second;
    ret.second += freq * first;
  }

  return ret;
}

int main(int argc, char **argv) {
  auto [win1, win2] = doStep(8, 0, 7, 0, 0);
  std::cout << "Result 2 " << std::max(win1, win2) << "\n";
  return 0;
}

// 346642902541848
// 27674034218179
// 632979211251440