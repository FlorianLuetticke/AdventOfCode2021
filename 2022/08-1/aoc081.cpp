#include <algorithm>
#include <charconv>
#include <cstddef>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

std::pair<long, long> splitToInt(const std::string& in, const std::string& sep)
{
    auto                  middle = in.find(sep);
    std::pair<long, long> result;
    auto [_, ec] = std::from_chars(in.data(), in.data() + middle, result.first);
    auto [__, ec2] =
        std::from_chars(in.data() + middle + sep.length(), in.data() + in.size(), result.second);
    if(ec != std::errc() || ec2 != std::errc()) {
        throw std::system_error(std::make_error_code(ec));
    }
    return result;
}
std::pair<std::string, std::string> split(const std::string& in, const std::string& sep)
{
    auto middle = in.find(sep);
    if(middle == in.npos) {
        std::cout << in << std::endl;
        throw std::logic_error("can't split");
    }
    return {in.substr(0, middle), in.substr(middle + sep.length())};
}

std::vector<std::string> csvToStringVec(const std::string& in, const std::string& sep)
{
    std::vector<std::string> resultVec;
    auto                     middle   = in.find(sep);
    size_t                   curStart = 0;

    while(middle != in.npos) {
        resultVec.push_back(in.substr(curStart, middle - curStart));
        curStart = middle + sep.length();
        middle   = in.find(sep, curStart);
    }
    resultVec.push_back(in.substr(curStart, in.size() - curStart));
    return resultVec;
}
class TreeGrid
{

public:
    TreeGrid(std::ifstream& input)
    {

        while(!input.eof()) {
            std::string line;
            std::getline(input, line);
            if(line.size() < 2)
                continue;
            std::vector<int> addVector;
            std::vector<int> addVisibleVector;
            for(auto& val : line) {
                addVector.push_back(int(val - '0'));
                addVisibleVector.push_back(0);
            }
            height.push_back(addVector);
            visible.push_back(addVisibleVector);
        }
        x                       = height.size();
        y                       = height.at(0).size();
        visible.front().front() = visible.front().back() = visible.back().front() =
            visible.back().back()                        = 1;
    }
    size_t                        x = 0;
    size_t                        y = 0;
    std::vector<std::vector<int>> height;
    std::vector<std::vector<int>> visible;

    size_t scenicScore(long startx, long starty)
    {

        auto   curX = startx, curY = starty;
        size_t dirCountNX = 0;
        while(curX > 0 && curY > 0 && curX < long(x - 1) && curY < long(y - 1) &&
              (height.at(curX).at(curY) < height.at(startx).at(starty) ||
               (starty == curY && startx == curX)))
        {
            dirCountNX++;
            curX--;
        }
        curX = startx, curY = starty;
        size_t dirCountPX = 0;
        while(curX > 0 && curY > 0 && curX < long(x - 1) && curY < long(y - 1) &&
              (height.at(curX).at(curY) < height.at(startx).at(starty) ||
               (starty == curY && startx == curX)))
        {
            dirCountPX++;
            curX++;
        }
        curX = startx, curY = starty;
        size_t dirCountNY = 0;
        while(curX > 0 && curY > 0 && curX < long(x - 1) && curY < long(y - 1) &&
              (height.at(curX).at(curY) < height.at(startx).at(starty) ||
               (starty == curY && startx == curX)))
        {
            dirCountNY++;
            curY--;
        }
        curX = startx, curY = starty;
        size_t dirCountPY = 0;
        while(curX > 0 && curY > 0 && curX < long(x - 1) && curY < long(y - 1) &&
              (height.at(curX).at(curY) < height.at(startx).at(starty) ||
               (starty == curY && startx == curX)))
        {
            dirCountPY++;
            curY++;
        }
        std::cout << "Tree " << startx << " " << starty << " has scores " << dirCountNX << " "
                  << dirCountPX << " " << dirCountNY << " " << dirCountPY << " =  "
                  << dirCountNX * dirCountPX * dirCountNY * dirCountPY << "\n";
        return dirCountNX * dirCountPX * dirCountNY * dirCountPY;
    }
};

int main(int argc, char** argv)
{
    std::ifstream input("input.txt", std::ifstream::in);
    if(!input.is_open()) {
        std::cout << "Error opening\n";
        return -1;
    }
    TreeGrid grid(input);

    for(size_t curX = 1; curX < grid.x - 1; ++curX) {
        int lastMaxHeight                    = grid.height.at(curX).at(0);
        int lastReverseMaxHeight             = grid.height.at(curX).at(grid.y - 1);
        grid.visible.at(curX).at(0)          = 1;
        grid.visible.at(curX).at(grid.y - 1) = 1;
        for(size_t curY = 1; curY < grid.y - 1; ++curY) {
            auto& val        = grid.height.at(curX).at(curY);
            auto& valReverse = grid.height.at(curX).at(grid.y - 1 - curY);
            if(val > lastMaxHeight) {
                grid.visible.at(curX).at(curY) = 1;
                lastMaxHeight                  = val;
            }
            if(valReverse > lastReverseMaxHeight) {
                grid.visible.at(curX).at(grid.y - 1 - curY) = 1;
                lastReverseMaxHeight                        = valReverse;
            }
        }
    }

    for(size_t curY = 1; curY < grid.y - 1; ++curY) {
        int lastMaxHeight                    = grid.height.at(0).at(curY);
        int lastReverseMaxHeight             = grid.height.at(grid.x - 1).at(curY);
        grid.visible.at(0).at(curY)          = 1;
        grid.visible.at(grid.x - 1).at(curY) = 1;
        for(size_t curX = 1; curX < grid.x - 1; ++curX) {
            auto& val        = grid.height.at(curX).at(curY);
            auto& valReverse = grid.height.at(grid.x - 1 - curX).at(curY);
            if(val > lastMaxHeight) {
                grid.visible.at(curX).at(curY) = 1;
                lastMaxHeight                  = val;
            }
            if(valReverse > lastReverseMaxHeight) {
                grid.visible.at(grid.x - 1 - curX).at(curY) = 1;
                lastReverseMaxHeight                        = valReverse;
            }
        }
    }
    int    visibleCount   = 0;
    size_t maxScenicScore = 0;
    for(size_t curY = 0; curY < grid.y; ++curY) {
        for(size_t curX = 0; curX < grid.x; ++curX) {
            visibleCount += grid.visible.at(curX).at(curY);
            maxScenicScore = std::max(maxScenicScore, grid.scenicScore(curX, curY));
        }
    }

    std::cout << "visible " << visibleCount << "\n";
    std::cout << "maxScenicScore " << maxScenicScore << "\n";
    return 0;
}
