#include <array>
#include <fstream>
#include <iostream>
#include <limits>
int main(int argc, char **argv) {
  std::ifstream input;
  input.open("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  int lastDepth = std::numeric_limits<int>::max();
  int countOfLarger = 0;
  int inputCount = 0;
  std::array<int, 3> last;
  while (!input.eof()) {
    int depth;
    input >> depth;
    if (input.fail() || input.eof()) {
      break;
    }
    if (inputCount >= 3 && depth > last[inputCount % 3]) {
      ++countOfLarger;
    }
    last[inputCount % 3] = depth;
    ++inputCount;
  }
  std::cout << "\nResult " << countOfLarger << "\n";
  return 0;
}
