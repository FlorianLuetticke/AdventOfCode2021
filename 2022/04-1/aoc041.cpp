#include <algorithm>
#include <charconv>
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
std::pair<long, long> splitToInt(const std::string& in, const std::string& sep = "..")
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
std::pair<std::string, std::string> split(const std::string& in, const std::string& sep = "..")
{
    auto middle = in.find(sep);
    if(middle == in.npos) {
        std::cout << in << std::endl;
        throw std::logic_error("can't split");
    }
    return {in.substr(0, middle), in.substr(middle + sep.length())};
}
class CleaningSegments
{
public:
    CleaningSegments(std::ifstream& input)
    {
        std::string line;
        std::getline(input, line);

        auto [lhs, rhs] = split(line, ",");
        elf1Segments    = splitToInt(lhs, "-");
        elf2Segments    = splitToInt(rhs, "-");
    }

    std::pair<long, long> elf1Segments;
    std::pair<long, long> elf2Segments;

    bool completeOverlap() const
    {
        return (elf1Segments.first <= elf2Segments.first &&
                elf1Segments.second >= elf2Segments.second) ||
               (elf2Segments.first <= elf1Segments.first &&
                elf2Segments.second >= elf1Segments.second);
    }

    bool partialOverlap() const
    {
        return elf1Segments.first <= elf2Segments.second &&
               elf2Segments.first <= elf1Segments.second;
    }
};
int main(int argc, char** argv)
{
    std::ifstream input("input.txt", std::ifstream::in);
    if(!input.is_open()) {
        std::cout << "Error opening\n";
        return -1;
    }
    std::vector<CleaningSegments> segments;
    while(!input.eof()) {
        segments.emplace_back(input);
    }

    auto score = std::count_if(segments.begin(), segments.end(), [](const CleaningSegments& in) {
        return in.completeOverlap();
    });
    std::cout << "Score1 " << score << std::endl;
    score = std::count_if(segments.begin(), segments.end(), [](const CleaningSegments& in) {
        return in.partialOverlap();
    });
    std::cout << "Score2 " << score << std::endl;
    return 0;
}
