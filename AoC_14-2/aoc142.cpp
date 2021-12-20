#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <ostream>
#include <unordered_map>

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::map<std::pair<char, char>, char> insertionRules;
  std::unordered_map<char, size_t> elemCount;
  std::map<std::pair<char, char>, size_t> polymerePairs;
  std::string in;
  std::getline(input, in);

  elemCount[in[0]]++;
  for (size_t i = 1; i < in.size(); ++i) {
    polymerePairs[std::make_pair(in[i - 1], in[i])]++;
    elemCount[in[i]]++;
  }
  std::getline(input, in);
  while (true) {
    std::string in;
    std::getline(input, in);
    if (in.size() < 7)
      break;
    insertionRules[std::make_pair(in[0], in[1])] = in.at(6);
  }

  for (auto [pair, target] : insertionRules) {
    std::cout << pair.first << " and " << pair.second << " go to " << target
              << std::endl;
  }

  for (int i = 0; i < 40; ++i) {
    std::map<std::pair<char, char>, size_t> newPolymerePairs;
    for (auto [pair, count] : polymerePairs) {
      if (auto insertIt = insertionRules.find(pair);
          insertIt != insertionRules.end()) {
        newPolymerePairs[std::make_pair(pair.first, insertIt->second)] += count;
        newPolymerePairs[std::make_pair(insertIt->second, pair.second)] +=
            count;
        elemCount[insertIt->second] += count;
      }
    }
    polymerePairs = std::move(newPolymerePairs);
    std::cout << "Step " << i + 1 << "\n";
    for (auto [pair, count] : polymerePairs) {
      std::cout << pair.first << pair.second << ": " << count << std::endl;
    }
  }

  size_t minElemCount = std::numeric_limits<size_t>::max();
  size_t maxElemCount = 0;
  for (auto [c, val] : elemCount) {
    std::cout << "value " << c << " is present " << val << std::endl;
    maxElemCount = std::max(val, maxElemCount);
    minElemCount = std::min(val, minElemCount);
  }

  std::cout << "Result " << maxElemCount - minElemCount << std::endl;
  return 0;
}
