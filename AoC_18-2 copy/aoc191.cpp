#include <algorithm>
#include <array>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <optional>
#include <ostream>
#include <set>
#include <stdexcept>
#include <type_traits>

class Permutation {

public:
  int x_dim = 0;
  int y_dim = 1;
  int z_dim = 2;
  int parity = 1;
  constexpr Permutation(int x, int y, int z, int par)
      : x_dim(x), y_dim(y), z_dim(z), parity(par) {
    if (x_dim == y_dim || x_dim == z_dim || y_dim == z_dim || x_dim < 0 ||
        y_dim < 0 || z_dim < 0 || x_dim > 2 || y_dim > 2 || z_dim > 2) {
      throw std::logic_error("something wrong in permute");
    }
  }

  Permutation operator+(const Permutation &p) const {
    switch (x_dim) {
    case 0:
      switch (y_dim) {
      case 1:
        return Permutation(p.x_dim, p.y_dim, p.z_dim, parity * p.parity);
      case 2:
        return Permutation(p.x_dim, p.z_dim, p.y_dim, parity * p.parity);
      default:
        throw(std::logic_error("err"));
      }
      break;
    case 1:
      switch (y_dim) {
      case 0:
        return Permutation(p.y_dim, p.x_dim, p.z_dim, parity * p.parity);
      case 2:
        return Permutation(p.y_dim, p.z_dim, p.x_dim, parity * p.parity);
      default:
        throw(std::logic_error("err"));
      }
    case 2:
      switch (y_dim) {
      case 0:
        return Permutation(p.z_dim, p.x_dim, p.y_dim, parity * p.parity);
      case 1:
        return Permutation(p.z_dim, p.y_dim, p.x_dim, parity * p.parity);
      default:
        throw(std::logic_error("err"));
      }
    default:
      throw(std::logic_error("err"));
    }
    throw(std::logic_error("err"));
  }
  friend std::ostream &operator<<(std::ostream &stream, const Permutation &p) {
    stream << "Perm{" << std::setw(1) << p.x_dim << "," << p.y_dim << ","
           << p.z_dim << "}";
    return stream;
  }
};
static constexpr std::array<Permutation, 6> permutations{
    Permutation{0, 1, 2, 1},
    {0, 2, 1, -1},
    {1, 0, 2, -1},
    {1, 2, 0, 1},
    {2, 0, 1, 1},
    {2, 1, 0, -1}};

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

struct Point {
  int x, y, z;
  Point(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {}

  bool operator<(const Point &other) const {
    return x < other.x || (x == other.x && y < other.y) ||
           (x == other.x && y == other.y && z < other.z);
  }
  bool operator==(const Point &other) const {
    return (x == other.x && y == other.y && z == other.z);
  }
  friend std::ostream &operator<<(std::ostream &stream, const Point &p) {
    stream << "P{" << std::setw(3) << p.x << "," << std::setw(3) << p.y << ","
           << std::setw(3) << p.z << "}";
    return stream;
  }
  Point operator-(const Point &rhs) const {
    return Point(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  Point operator+(const Point &rhs) const {
    return Point(x + rhs.x, y + rhs.y, z + rhs.z);
  }
  void abs() {
    x = std::abs(x);
    y = std::abs(y);
    z = std::abs(z);
  }

  Point mirror(bool x_dim, bool y_dim, bool z_dim) const {
    return Point(x_dim ? -x : x, y_dim ? -y : y, z_dim ? -z : z);
  }
  Point permute(const Permutation &p) const {

    int newX = 0, newY = 0, newZ = 0;
    switch (p.x_dim) {
    case 0:
      newX = x;
      switch (p.y_dim) {
      case 1:
        newY = y;
        newZ = z;
        break;
      case 2:
        newY = z;
        newZ = y;
        break;
      }
      break;
    case 1:
      newX = y;
      switch (p.y_dim) {
      case 0:
        newY = x;
        newZ = z;
        break;
      case 2:
        newY = z;
        newZ = x;
        break;
      }
      break;
    case 2:
      newX = z;
      switch (p.y_dim) {
      case 0:
        newY = x;
        newZ = y;
        break;
      case 1:
        newY = y;
        newZ = x;
        break;
      }
      break;
    }
    return Point(newX, newY, newZ);
  }
};

class Counter {
public:
  Point tmpVal{0, 0, 0};
  int count = 0;
  class CountIterator {
    Counter &cnt;

  public:
    CountIterator(Counter &c) : cnt(c) {}
    CountIterator &operator++() { return *this; }
    CountIterator &operator++(int) { return *this; }
    Point &operator*() {
      cnt.count += 1;
      return cnt.tmpVal;
    }
    void operator=(const Point &val) {
      cnt.count++;
      cnt.tmpVal = val;
    }
  };
  CountIterator begin() { return CountIterator(*this); }
};
class Scanner;
struct Solution {
  int flipX = 0;
  int flipY = 0;
  int flipZ = 0;
  Point offset{0, 0, 0};
  Permutation permutation = permutations[0];

  Solution &operator+=(const Solution &other) {
    offset =
        offset + other.offset.permute(permutation).mirror(flipX, flipY, flipZ);
    Point newFlip(other.flipX, other.flipY, other.flipZ);
    newFlip = newFlip.permute(permutation);
    flipX = (flipX + newFlip.x) % 2;
    flipY = (flipY + newFlip.y) % 2;
    flipZ = (flipZ + newFlip.z) % 2;
    permutation = permutation + other.permutation;
    return *this;
  }

  Point project(const Point &p) const {
    return offset + p.permute(permutation).mirror(flipX, flipY, flipZ);
  }
};

class Scanner {
public:
  std::vector<Point> inputPositions;

  std::vector<std::vector<std::vector<Point>>> permutedAbsRelDistances;

  Scanner(std::ifstream &input) {

    while (true) {
      std::string line;
      std::getline(input, line);
      if (line.size() < 3) {
        break;
      }
      auto pt = csvToIntVec(line);
      inputPositions.emplace_back(pt.at(0), pt.at(1), pt.at(2));
    }
    std::cout << "got " << inputPositions.size() << "inputs"
              << "\n";

    permutedAbsRelDistances = std::vector(
        6, std::vector(inputPositions.size(), std::vector<Point>()));
    for (size_t i = 0; i < inputPositions.size(); ++i) {
      for (size_t p = 0; p < permutations.size(); ++p) {
        permutedAbsRelDistances.at(p).at(i).reserve(inputPositions.size() - 1);
      }
      for (size_t j = 0; j < inputPositions.size(); ++j) {
        if (i == j)
          continue;
        auto difference = inputPositions[i] - inputPositions[j];
        difference.abs();
        for (size_t p = 0; p < permutations.size(); ++p) {
          permutedAbsRelDistances[p][i].push_back(
              difference.permute(permutations[p]));
        }
      }
      for (size_t p = 0; p < permutations.size(); ++p) {
        std::sort(permutedAbsRelDistances[p][i].begin(),
                  permutedAbsRelDistances[p][i].end());
      }
    }

    // for (size_t p = 0; p < permutations.size(); ++p) {
    //   std::cout << "Permutation " << p << std::endl;
    //   for (size_t i = 0; i < inputPositions.size(); ++i) {
    //     for (auto val : permutedAbsRelDistances[p][i]) {
    //       std::cout << val << ",";
    //     }
    //     std::cout << std::endl;
    //   }
    // }
  }

  std::optional<Solution> mayHaveOverlap(const Scanner &other) {

    for (size_t i = 0; i < inputPositions.size(); ++i) {
      for (size_t other_i = 0; other_i < other.inputPositions.size();
           ++other_i) {
        for (size_t p = 0; p < permutations.size(); ++p) {
          auto &relCoords = permutedAbsRelDistances[0][i];
          auto &otherRelCoords = other.permutedAbsRelDistances[p][other_i];
          Counter cnt;
          std::set_intersection(relCoords.begin(), relCoords.end(),
                                otherRelCoords.begin(), otherRelCoords.end(),
                                cnt.begin());

          if (cnt.count >= 11) {
            auto val = checkOverlapCandidate(i, other_i, p, other);
            if (val) {
              return val;
            }
          }
        }
      }
    }
    return std::nullopt; // if you leave that, you get a compiler bug.
  }

  std::optional<Solution> checkOverlapCandidate(int i, int other_i, int permId,
                                                const Scanner &other) {

    std::vector<Point> absDifferences;
    for (size_t j = 0; j < inputPositions.size(); ++j)
      absDifferences.push_back(inputPositions[i] - inputPositions[j]);
    std::sort(absDifferences.begin(), absDifferences.end());
    auto perm = permutations[permId];

    for (auto flipX = 0; flipX <= 1; ++flipX) {
      for (auto flipY = 0; flipY <= 1; ++flipY) {
        for (auto flipZ = 0; flipZ <= 1; ++flipZ) {
          if ((flipX + flipY + flipZ) % 2 == 0 && perm.parity != 1) {
            continue;
          }
          std::vector<Point> otherAbsDifferences;
          for (size_t j = 0; j < other.inputPositions.size(); ++j)
            otherAbsDifferences.push_back(
                (other.inputPositions[other_i] - other.inputPositions[j])
                    .permute(perm)
                    .mirror(flipX, flipY, flipZ));
          std::sort(otherAbsDifferences.begin(), otherAbsDifferences.end());

          Counter cnt;
          std::set_intersection(absDifferences.begin(), absDifferences.end(),
                                otherAbsDifferences.begin(),
                                otherAbsDifferences.end(), cnt.begin());

          if (cnt.count >= 12) {
            Solution s;
            s.flipX = flipX;
            s.flipY = flipY;
            s.flipZ = flipZ;
            s.offset = inputPositions[i] -
                       other.inputPositions[other_i].permute(perm).mirror(
                           flipX, flipY, flipZ);
            s.permutation = perm;
            return s;
          }
        }
      }
    }
    return std::nullopt;
  }
};

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }

  std::vector<Scanner> scanners;
  while (true) {
    std::string line;
    std::getline(input, line);
    if (line.size() < 2 || (line.size() > 2 && line[0] != '-')) {
      break;
    }

    scanners.push_back(input);
  }

  std::vector<std::optional<Solution>> solution0(scanners.size(), std::nullopt);
  std::vector<bool> finished(scanners.size(), false);
  solution0[0] = Solution();
  size_t solved = 1;
  while (solved < solution0.size()) {
    for (auto fromIdx = 0; fromIdx < solution0.size(); ++fromIdx) {
      if (!solution0[fromIdx].has_value() || finished[fromIdx]) {
        continue;
      }
      for (auto toIdx = 0; toIdx < solution0.size(); ++toIdx) {
        if (solution0[toIdx].has_value()) {
          continue;
        }
        auto val = scanners.at(fromIdx).mayHaveOverlap(scanners.at(toIdx));
        if (val) {
          std::cout << "found " << fromIdx << " to " << toIdx << " solution "
                    << val->flipX << " " << val->flipY << " " << val->flipZ
                    << " " << val->permutation << " " << val->offset << "\n";
          solution0[toIdx] = solution0[fromIdx];
          solution0[toIdx].value() += val.value();
          solved++;
        }
      }
      finished[fromIdx];
    }
  }

  std::set<Point> s;
  for (auto i = 0; i < solution0.size(); ++i) {
    for (auto j = 0; j < scanners[i].inputPositions.size(); ++j) {
      s.insert(solution0[i].value().project(scanners[i].inputPositions[j]));
    }
  }

  std::cout << "Result 1 " << s.size() << "\n";
  int max = 0;
  for (auto i = 0; i < solution0.size() - 1; ++i) {
    for (auto j = i + 1; j < solution0.size(); ++j) {
      auto diff = (solution0[i]->offset - solution0[j]->offset);
      diff.abs();
      max = std::max(max, diff.x + diff.y + diff.z);
    }
  }
  std::cout << "Result 2 " << max << "\n";
  return 0;
}
