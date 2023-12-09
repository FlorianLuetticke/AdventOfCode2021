#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

struct Path {
  std::vector<std::string> visited;
  bool smallVisitedTwice = false;
};

bool caveSmallAndVisited(const std::string &cave, const Path &path) {
  return std::islower(cave[0]) &&
         path.visited.end() !=
             std::find(path.visited.begin(), path.visited.end(), cave);
}

template <typename T>
void printVec(const std::vector<T> &in, const std::string &sep = ",") {
  std::cout << "{";
  for (size_t i = 0; i < in.size(); ++i) {
    std::cout << in[i] << (i == in.size() - 1 ? "}" : sep);
  }
}

std::map<std::string, std::vector<std::string>> caveMap;

std::pair<size_t, std::vector<Path>>
advancePathsIfNotAtEnd(std::vector<Path> &&in) {
  std::vector<Path> ret;
  size_t finished = 0;
  for (auto &path : in) {
    for (const auto &nextCave : caveMap.at(path.visited.back())) {
      if (nextCave == "end") {
        ++finished;
        continue;
      }
      bool extraSmallVisit = path.smallVisitedTwice;
      if (caveSmallAndVisited(nextCave, path)) {
        if (path.smallVisitedTwice)
          continue;
        extraSmallVisit = true;
      }
      ret.push_back(path);
      ret.back().visited.push_back(nextCave);
      ret.back().smallVisitedTwice = extraSmallVisit;
    }
  }
  return {finished, std::move(ret)};
}

int main(int argc, char **argv) {
  std::ifstream input("input.txt", std::ifstream::in);
  if (!input.is_open()) {
    std::cout << "Error opening\n";
    return -1;
  }
  std::string in;
  while (input >> in) {
    auto sepPos = in.find('-');
    auto origin = in.substr(0, sepPos);
    auto target = in.substr(sepPos + 1);
    std::cout << "connection " << in << " from " << origin << " to " << target
              << "\n";
    if (origin != "end" && target != "start")
      caveMap[origin].push_back(target);
    if (target != "end" && origin != "start")
      caveMap[target].push_back(origin);
  }

  std::vector<Path> curPaths(1);
  curPaths.back().visited.push_back("start");
  size_t totalFinished = 0;
  size_t currentFinished = 0;
  while (curPaths.size() > 0) {
    std::tie(currentFinished, curPaths) =
        advancePathsIfNotAtEnd(std::move(curPaths));

    std::cout << "After iteration: finished " << currentFinished
              << ", remaining paths " << curPaths.size() << " \n";
    totalFinished += currentFinished;
    // for (auto path : curPaths) {
    //   printVec(path.visited);
    //   std::cout << "\n";
    // }
  }

  std::cout << "Cave network:\n";
  for (auto &[name, cave] : caveMap) {
    std::cout << name << " connected to ";
    printVec(cave);
    std::cout << "\n";
  }
  std::cout << "Result " << totalFinished << "\n";
  return 0;
}
