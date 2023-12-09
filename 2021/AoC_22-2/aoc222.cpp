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

class Region {
  long xMin, xMax, yMin, yMax, zMin, zMax;
  bool on;

public:
  bool isOn() { return on; }
  Region(const std::string &in) {
    auto [onOff, rest1] = split(in, " x=");
    auto [xString, rest2] = split(rest1, ",y=");
    auto [yString, zString] = split(rest2, ",z=");
    std::cout << onOff << "\n";
    std::cout << xString << "\n";
    std::cout << yString << "\n";
    std::cout << zString << "\n";
    on = onOff[1] == 'n';
    std::tie(xMin, xMax) = splitToInt(xString, "..");
    std::tie(yMin, yMax) = splitToInt(yString, "..");
    std::tie(zMin, zMax) = splitToInt(zString, "..");
  }
  Region(long _xMin, long _xMax, long _yMin, long _yMax, long _zMin, long _zMax,
         bool _on)
      : xMin(_xMin), xMax(_xMax), yMin(_yMin), yMax(_yMax), zMin(_zMin),
        zMax(_zMax), on(_on) {}

  bool encloses(const Region &other) const {
    return xMin <= other.xMin && yMin <= other.yMin && zMin <= other.zMin &&
           xMax >= other.xMax && yMax >= other.yMax && zMax >= other.zMax;
  }

  bool overlaps(const Region &other) const {
    return xMin <= other.xMax && yMin <= other.yMax && zMin <= other.zMax &&
           xMax >= other.xMin && yMax >= other.yMin && zMax >= other.zMin;
  }

  std::vector<Region> splitUpIfNeeded(const Region &other) {
    if (other.encloses(*this)) {
      return {};
    }
    if (!overlaps(other)) {
      return {*this};
    }

    std::vector<long> xLimits;
    xLimits.push_back(xMin);
    if (other.xMin > xMin) {
      xLimits.push_back(other.xMin);
    }
    if (other.xMax < xMax) {
      xLimits.push_back(other.xMax + 1);
    }
    xLimits.push_back(xMax + 1);
    std::vector<long> yLimits;
    yLimits.push_back(yMin);
    if (other.yMin > yMin) {
      yLimits.push_back(other.yMin);
    }
    if (other.yMax < yMax) {
      yLimits.push_back(other.yMax + 1);
    }
    yLimits.push_back(yMax + 1);

    std::vector<long> zLimits;
    zLimits.push_back(zMin);
    if (other.zMin > zMin) {
      zLimits.push_back(other.zMin);
    }
    if (other.zMax < zMax) {
      zLimits.push_back(other.zMax + 1);
    }
    zLimits.push_back(zMax + 1);
    std::vector<Region> ret;
    for (size_t i = 0; i < xLimits.size() - 1; ++i) {
      for (size_t j = 0; j < yLimits.size() - 1; ++j) {
        for (size_t k = 0; k < zLimits.size() - 1; ++k) {
          ret.emplace_back(xLimits[i], xLimits[i + 1] - 1, yLimits[j],
                           yLimits[j + 1] - 1, zLimits[k], zLimits[k + 1] - 1,
                           on);
          if (ret.back().overlaps(other)) {
            if (!other.encloses(ret.back())) {
              std::cout << "seems to be an error here";
            }
            ret.pop_back();
          }
        }
      }
    }
    return ret;
  }
  friend std::ostream &operator<<(std::ostream &stream, const Region &p) {
    stream << "Reg{" << p.xMin << " to " << p.xMax << ", " << p.yMin << " to "
           << p.yMax << ", " << p.zMin << " to " << p.zMax << "} ";
    return stream;
  }
  long sum() {
    return (xMax + 1 - xMin) * (yMax + 1 - yMin) * (zMax + 1 - zMin);
  }
};

int main(int argc, char **argv) {

  std::ifstream input("inputTest.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::set<std::tuple<long, long, long>> s;
  std::vector<Region> vec;
  while (true) {
    std::string line;
    std::getline(input, line);
    if (line.size() < 2) {
      break;
    }
    Region nextRegion(line);
    std::cout << " New Region " << nextRegion << " " << nextRegion.isOn()
              << " old size " << vec.size() << "\n";
    std::vector<Region> newVec;
    for (auto inReg : vec) {
      auto additional = inReg.splitUpIfNeeded(nextRegion);
      if (additional.size() != 1)
        std::cout << "Region " << inReg << " split by " << nextRegion << "\n";
      for (auto outReg : additional) {
        if (additional.size() != 1)
          std::cout << "Results in " << outReg << "\n";
        newVec.push_back(outReg);
      }
    }
    vec = newVec;
    if (nextRegion.isOn()) {

      vec.push_back(nextRegion);
    }
  }
  long sum = 0;
  for (auto val : vec) {
    sum += val.sum();
  }
  std::cout << "Result 2 " << sum << "\n";
  return 0;
}

// 346642902541848
// 27674034218179
// 632979211251440