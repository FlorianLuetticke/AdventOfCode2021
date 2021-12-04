#include <algorithm>
#include <array>
#include <bitset>
#include <charconv>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <system_error>
#include <vector>
// 1 wins on same amount
int findMostCommonNthBit(const std::vector<std::string> input, size_t n) {
  int nOnes = 0;
  for (const auto &val : input)
    nOnes += -1 + 2 * (val[n] == '1');
  return nOnes >= 0;
}

size_t intFromBinaryString(const std::string &val) {
  return std::accumulate(val.begin(), val.end(), int(0),
                         [](int res, char c) { return 2 * res + (c == '1'); });
}

class Board {
public:
  std::array<int, 25> boardData = {};
  int calledCodes = 0;
  std::array<int, 10> correct;
  bool win = false;
  Board(std::ifstream &input) {
    correct[0] = 0x1F;
    correct[5] = 0x108421;
    for (size_t i : {1, 2, 3, 4}) {
      correct[i] = correct[0] << (5 * i);
      correct[5 + i] = correct[5] << (i);
    }
    for (auto &val : boardData) {
      if (!(input >> val)) {
        throw std::out_of_range("not enougth data");
      }
    }
  }
  void called(int number) {
    auto foundIt = std::find(boardData.begin(), boardData.end(), number);
    if (foundIt != boardData.end()) {

      auto index = foundIt - boardData.begin();
      calledCodes = calledCodes | (1 << index);
      if (correct.end() !=
          std::find_if(correct.begin(), correct.end(), [this](int value) {
            return ((calledCodes & value) == value);
          })) {
        win = true;
      }
    }
  }
  int score() {
    int score = 0;
    for (int i = 0; i < 25; ++i) {
      if ((calledCodes & (1 << i)) == 0) {
        score += boardData[i];
      }
    }
    return score;
  }
};

std::vector<int> csvToIntVec(const std::string &input) {
  auto curPos = input.data();
  auto end = input.data() + input.size();
  std::vector<int> resultVec;
  while (curPos < end) {
    int result = 0;
    auto [ptr, ec] = std::from_chars(curPos, end, result);
    if (ec == std::errc()) {
      resultVec.push_back(result);
      if (*ptr == ',') {
        ++ptr;
      } else {
        return resultVec;
      }
      curPos = ptr;
    } else {
      throw std::system_error(std::make_error_code(ec));
    }
  }
  return resultVec;
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::string line;
  std::getline(input, line);
  std::vector<int> randomNumbers = csvToIntVec(line);

  std::vector<Board> boards;
  while (true) {
    try {
      boards.emplace_back(input);
    } catch (std::out_of_range &e) {
      std::cout << "read " << std::dec << boards.size() << " boards\n";
      break;
    }
  }
  std::set<size_t> won;
  for (auto number : randomNumbers) {
    for (size_t i = 0; i < boards.size(); ++i) {
      boards[i].called(number);
      if (boards[i].win) {
        won.insert(i);
        if (won.size() == boards.size()) {
          std::cout << boards[i].score() << " * " << number << " -> "
                    << boards[i].score() * number << "\n";
          return 0;
        }
      }
    }
  }

  std::cout << "No board wins?\n";
  return 0;
}
