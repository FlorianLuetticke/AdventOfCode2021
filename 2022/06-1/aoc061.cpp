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
class UniqueCharCounter
{

public:
    std::vector<long> charCount   = {256, 0};
    long              uniqueChars = 0;
    void              addChar(char in)
    {
        charCount[size_t(in)] += 1;
        if(charCount[size_t(in)] == 1) {
            uniqueChars += 1;
        }
        else if(charCount[size_t(in)] == 2) {
            uniqueChars -= 1;
        }
        std::cout << "Add " << in << ", now unique: " << uniqueChars << "\n";
    }

    void removeChar(char in)
    {
        charCount[size_t(in)] -= 1;
        if(charCount[size_t(in)] == 1) {
            uniqueChars += 1;
        }
        else if(charCount[size_t(in)] == 0) {
            uniqueChars -= 1;
        }
        std::cout << "remove " << in << ", now unique: " << uniqueChars << "\n";
    }
};
int main(int argc, char** argv)
{
    std::ifstream input("input.txt", std::ifstream::in);
    if(!input.is_open()) {
        std::cout << "Error opening\n";
        return -1;
    }
    const int requiredUnique = 14;

    UniqueCharCounter                charCounter;
    std::array<char, requiredUnique> charBuffer;
    size_t                           count = 0;

    for(unsigned i = 0; i < requiredUnique; ++i) {
        char c = input.get();
        charCounter.addChar(c);
        charBuffer[count % requiredUnique] = c;
        count++;
    }

    while(!input.eof() && charCounter.uniqueChars != requiredUnique) {

        charCounter.removeChar(charBuffer[count % requiredUnique]);

        char c = input.get();
        charCounter.addChar(c);
        charBuffer[count % requiredUnique] = c;
        count++;
    }
    std::cout << "The count is " << count << std::endl;

    return 0;
}
