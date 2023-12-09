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
        resultVec.push_back(in.substr(curStart, middle));
        curStart = middle + sep.length();
        middle   = in.find(sep, curStart);
    }
    resultVec.push_back(in.substr(curStart, in.size()));
    return resultVec;
}

class CrateStack
{
public:
    CrateStack(std::ifstream& input)
    {
        std::string line;
        std::getline(input, line);
        stacks.resize((line.size() + 1) / 4);

        while(line.size() > 2) {
            if(line.find("[") != line.npos) {
                for(auto index = 0; index < stacks.size(); ++index) {
                    auto crate = line.at(4 * index + 1);

                    if(crate != ' ') {
                        stacks[index].push_back(crate);
                    }
                }
            }
            line.clear();
            std::getline(input, line);
        }
        for(auto& stack : stacks) {
            std::reverse(stack.begin(), stack.end());
            for(auto val : stack) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
    std::vector<std::vector<char>> stacks;

    void move(size_t number, size_t from, size_t to, bool asOne)
    {
        std::cout << "Command: " << number << " from " << from << " to " << to << "\n";
        if(asOne) {
            auto& src    = stacks.at(from - 1);
            auto& target = stacks.at(to - 1);
            target.insert(target.end(), src.end() - number, src.end());
            src.erase(src.end() - number, src.end());
        }
        else {
            for(auto count = 0; count < number; count++) {
                stacks.at(to - 1).push_back(stacks.at(from - 1).back());
                stacks.at(from - 1).pop_back();
            }
        }
    }
};
int main(int argc, char** argv)
{
    std::ifstream input("input.txt", std::ifstream::in);
    if(!input.is_open()) {
        std::cout << "Error opening\n";
        return -1;
    }

    CrateStack crateStack(input);

    while(!input.eof()) {
        std::string line;
        std::getline(input, line);
        std::cout << line << "\n";
        auto tokens = csvToStringVec(line, " ");
        crateStack.move(std::stoul(tokens.at(1)), std::stoul(tokens.at(3)),
                        std::stoul(tokens.at(5)), true);
    }
    for(auto& stack : crateStack.stacks) {
        std::cout << stack.back();
    }
    return 0;
}
