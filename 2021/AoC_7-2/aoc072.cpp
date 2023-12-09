#include <algorithm>
#include <array>
#include <bitset>
#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <system_error>
#include <vector>
const int daysToSimulate = 256;

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
  std::vector<int> crabs = csvToIntVec(line);

  auto func = [&crabs](int median) {
    size_t fuel = 0;
    for (int crabPos : crabs) {
      fuel += std::abs(crabPos - median) * (std::abs(crabPos - median) + 1) / 2;
    }
    return fuel;
  };

  auto [minIt, maxIt] = std::minmax_element(crabs.begin(), crabs.end());
  int x1 = *minIt, x2 = *maxIt;
  int fuel1 = func(x1), fuel2 = func(x2);

  while (x1 + 1 < x2) {
    if (fuel1 > fuel2) {
      x1 = (x1 + x2) / 2;
      fuel1 = func(x1);
    } else {
      x2 = (x1 + x2) / 2;
      fuel2 = func(x2);
    }
    std::cout << x1 << " " << x2 << " " << fuel1 << " " << fuel2 << "\n";
  }

  std::cout << "Result " << std::min(fuel1, fuel2) << "\n";
  return 0;
}
