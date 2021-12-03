#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
int main(int argc, char **argv) {
  std::ifstream input;
  input.open("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  int curDepth = 0;
  int curForward = 0;
  std::array<int, 3> last;
  while (!input.eof()) {
    std::string dir;
    int amount;
    input >> dir >> amount;
    if (input.fail() || input.eof()) {
      if (!input.eof()) {
        std::cout << "Return with fail\n";
      }
      break;
    }
    switch (dir[0]) {
    case 'f':
      curForward += amount;
      break;
    case 'u':
      curDepth -= amount;
      break;
    case 'd':
      curDepth += amount;
      break;
    default:
      throw std::logic_error(dir);
    }
  }
  std::cout << "\nResult " << curForward * curDepth << "\n";
  return 0;
}
