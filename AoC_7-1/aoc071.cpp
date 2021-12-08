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
  size_t middle = crabs.size() / 2;
  // bring one or both medians to middle;
  std::nth_element(crabs.begin(), crabs.begin() + middle, crabs.end());
  int median1 = crabs[middle];
  if (crabs.size() % 2 == 0) {
    std::nth_element(crabs.begin(), crabs.begin() + middle - 1,
                     crabs.begin() + middle);
  }
  int median2 = crabs.size() % 2 == 0 ? crabs[middle - 1] : median1;

  size_t fuel1 = 0, fuel2 = 0;
  for (int crabPos : crabs) {
    fuel1 += std::abs(crabPos - median1);
    fuel2 += std::abs(crabPos - median2);
  }
  std::cout << "Result " << std::min(fuel1, fuel2) << "\n";
  return 0;
}
