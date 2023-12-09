#include <algorithm>
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
#include <vector>
class Backpack
{
public:
    Backpack(std::ifstream& input)
    {
        std::string line;
        std::getline(input, line);

        std::cout << "Got backpack " << line << "\n";
        if(line.size() % 2 != 0) {
            throw std::logic_error("input error");
        }
        for(size_t i = 0; i < line.size() / 2; ++i) {
            compartment1.insert(line[i]);
            fullContent.insert(line[i]);
        }
        for(size_t i = line.size() / 2; i < line.size(); ++i) {
            compartment2.insert(line[i]);
            fullContent.insert(line[i]);
        }
    }
    std::set<char> fullContent;
    std::set<char> compartment1;
    std::set<char> compartment2;
    char           getCommon()
    {
        std::vector<char> out;
        std::set_intersection(compartment1.begin(), compartment1.end(), compartment2.begin(),
                              compartment2.end(), std::back_inserter(out));
        return out.at(0);
    }

    std::set<char> getCommonWithOther(const std::set<char>& other)
    {
        std::vector<char> out;
        std::set_intersection(fullContent.begin(), fullContent.end(), other.begin(), other.end(),
                              std::back_inserter(out));
        return std::set<char>(out.begin(), out.end());
    }
};
int main(int argc, char** argv)
{
    std::ifstream input("input.txt", std::ifstream::in);
    if(!input.is_open()) {
        std::cout << "Error opening\n";
        return -1;
    }
    std::vector<Backpack> backpacks;
    while(!input.eof()) {
        backpacks.emplace_back(input);
    }
    int score = 0;
    for(auto& pack : backpacks) {
        auto common = pack.getCommon();
        if(std::isupper(common)) {
            score += 27 + (common - 'A');
        }
        else {
            score += 1 + (common - 'a');
        }
    }
    std::cout << "Score1 " << score << std::endl;
    score = 0;
    for(auto i = 0; i < backpacks.size(); i += 3) {
        auto commonItems = backpacks[i].getCommonWithOther(backpacks[i + 1].fullContent);
        commonItems      = backpacks[i + 2].getCommonWithOther(commonItems);
        auto common      = *commonItems.begin();
        if(std::isupper(common)) {
            score += 27 + (common - 'A');
        }
        else {
            score += 1 + (common - 'a');
        }
    }
    std::cout << "Score2 " << score << std::endl;

    return 0;
}
