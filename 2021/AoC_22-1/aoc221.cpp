#include <algorithm>
#include <array>
#include <charconv>
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
#include <tuple>
#include <type_traits>
#include <utility>
std::pair<long, long> splitToInt(const std::string &in,
                                 const std::string &sep = "..") {
  auto middle = in.find(sep);
  std::pair<long, long> result;
  auto [_, ec] = std::from_chars(in.data(), in.data() + middle, result.first);
  auto [__, ec2] = std::from_chars(in.data() + middle + sep.length(),
                                   in.data() + in.size(), result.second);
  if (ec != std::errc() || ec2 != std::errc()) {
    throw std::system_error(std::make_error_code(ec));
  }
  return result;
}
std::pair<std::string, std::string> split(const std::string &in,
                                          const std::string &sep = "..") {
  auto middle = in.find(sep);
  if (middle == in.npos) {
    throw std::logic_error("can't split");
  }
  return {in.substr(0, middle), in.substr(middle + sep.length())};
}

int main(int argc, char **argv) {

  std::ifstream input("inputTest.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::set<std::tuple<long, long, long>> s;

  while (true) {
    std::string line;
    std::getline(input, line);
    if (line.size() < 2) {
      break;
    }

    // on x=-20..26,y=-36..17,z=-47..7
    auto [onOff, rest1] = split(line, " x=");
    auto [xString, rest2] = split(rest1, ",y=");
    auto [yString, zString] = split(rest2, ",z=");
    std::cout << line << "\n";
    std::cout << onOff << "\n";
    std::cout << xString << "\n";
    std::cout << yString << "\n";
    std::cout << zString << "\n";

    auto xVals = splitToInt(xString, "..");
    auto yVals = splitToInt(yString, "..");
    auto zVals = splitToInt(zString, "..");
    bool on = onOff[1] == 'n';
    for (auto x = std::max(-50l, xVals.first); x <= std::min(50l, xVals.second);
         ++x) {
      for (auto y = std::max(-50l, yVals.first);
           y <= std::min(50l, yVals.second); ++y) {
        for (auto z = std::max(-50l, zVals.first);
             z <= std::min(50l, zVals.second); ++z) {
          if (std::abs(x) > 50 || std::abs(y) > 50 || std::abs(z) > 50) {
            continue;
          }
          if (on) {
            s.emplace(x, y, z);
          } else {
            s.erase(std::make_tuple(x, y, z));
          }
        }
      }
    }
  }

  std::cout << "Result 2 " << s.size() << "\n";
  return 0;
}

// 346642902541848
// 27674034218179
// 632979211251440