#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <optional>
#include <ostream>
#include <set>
#include <stdexcept>
#include <unordered_map>
// int minX = 20, maxX = 30, minY = -10, maxY = -5;
int minX = 201, maxX = 230, minY = -99, maxY = -65;

bool checkSolution(int xVel, int yVel, bool debug = false) {
  if (debug)
    std::cout << " Check " << xVel << " " << yVel << "\n";
  int x = xVel, y = yVel;
  if (debug)
    std::cout << " position " << x << " " << y << "\n";
  while (!(x > maxX) && !(y < minY)) {
    if (x >= minX && x <= maxX && y >= minY && y <= maxY) {
      if (debug)
        std::cout << " found correct " << x << " " << y << "\n";
      return true;
    }

    --yVel;
    if (xVel > 0)
      --xVel;
    x += xVel;
    y += yVel;
    if (debug)
      std::cout << " position " << x << " " << y << "\n";
  }
  if (debug)
    std::cout << " found bad " << x << " " << y << "\n";
  return false;
}

int main(int argc, char **argv) {
  std::cout << "Result Part 1 " << (minY * (minY + 1)) / 2 << "\n";

  int maxYVelocity = std::abs(minY) - 1;
  int minYVelocity = -std::abs(minY);

  // easy solutions: minX - maxX x minY-maxY
  // all triangle numbers between minX-maxX x all  sinking solutions
  std::vector<std::pair<int, int>> solutions;
  for (auto x = minX; x <= maxX; ++x)
    for (auto y = minY; y <= maxY; ++y) {
      if (checkSolution(x, y)) {
        std::cout << "found solution " << x << " " << y << "\n";
        solutions.emplace_back(x, y);
      } else {
        std::cout << "incorrect solution " << x << " " << y << "\n";
      }
    }

  int minTriangleX = int(std::floor(std::sqrt(float(minX * 2))));
  while (minTriangleX * (minTriangleX + 1) / 2 < std::abs(minX)) {
    minTriangleX++;
  }
  int maxTriangleX =
      int(std::ceil(std::sqrt(2 * (float(maxX) + std::sqrt(float(maxX))))));
  while (maxTriangleX * (maxTriangleX + 1) / 2 > std::abs(maxX)) {
    maxTriangleX--;
  }
  std::cout << "triangle X " << minTriangleX << " to " << maxTriangleX << "\n";

  int maxTriangleY = int(std::ceil(std::sqrt(
      2 * (float(std::abs(minY)) + std::sqrt(float(std::abs(minY)))))));
  while (maxTriangleY * (maxTriangleY + 1) / 2 > std::abs(minY)) {
    maxTriangleY--;
  }
  // we only have maxTriangleY + 1 rounds if initial velocity is zero, but we
  // need minTriangleX to maxTriangleX for the highArc Solutions

  for (auto x = minTriangleX; x <= maxTriangleX; ++x) {
    auto yStart = x - (maxTriangleY + 1);
    std::cout << "for x=" << x << " triangle solutions yStart from " << yStart
              << " to " << std::abs(minY) - 1 << "\n";
    if (yStart > 0)
      yStart = (yStart + 1) / 2; // goes up and down
    for (auto y = yStart; y <= std::abs(minY) - 1; ++y) {
      if (checkSolution(x, y)) {
        std::cout << "found solution " << x << " " << y << "\n";
        solutions.emplace_back(x, y);
      } else {
        std::cout << "incorrect solution " << x << " " << y << "\n";
      }
    }
  }

  // normal search space:
  int xStart = minTriangleX;
  int xEnd = (maxX + 1) / 2;
  int yStart = (minY + 1) / 2;
  for (auto x = xStart; x <= xEnd; ++x) {
    auto yEnd = maxTriangleX - (maxTriangleY + 1);
    if (x <= maxTriangleX) {
      yEnd = x - (maxTriangleY + 1);
      if (yEnd > 0)
        yEnd = (yEnd + 1) / 2 - 1; // goes up and down
    }
    std::cout << "for x=" << x << " search y from " << yStart << " to " << yEnd
              << "\n";
    for (auto y = yStart; y <= yEnd; ++y) {
      if (checkSolution(x, y)) {
        std::cout << "found solution " << x << " " << y << "\n";
        solutions.emplace_back(x, y);
      }
    }
  }
  std::set<std::pair<int, int>> unique(solutions.begin(), solutions.end());
  bool val = checkSolution(8, 1);
  std::cout << "Solution part 2 " << unique.size() << " " << solutions.size()
            << " " << std::endl;
  return 0;
}
