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
  int depth;
  while (input >> depth) {
    if (depth > lastDepth) {
      ++countOfLarger;
    }
    lastDepth = depth;
  }
  std::cout << "\nResult " << countOfLarger << "\n";
  return 0;
}
