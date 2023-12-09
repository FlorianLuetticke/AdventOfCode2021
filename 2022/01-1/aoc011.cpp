#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>
int main(int argc, char** argv)
{
    std::ifstream input("input.txt", std::ifstream::in);
    if(!input.is_open()) {
        std::cout << "Error opening\n";
        return -1;
    }

    std::vector<int> caloriesPerElf;
    while(!input.eof()) {
        bool ok         = true;
        int  currentSum = 0;

        do {
            ok = true;
            std::string line;

            std::getline(input, line);
            // std::cout << line << "\n";
            if(line.size() <= 1) {
                ok = false;
            }
            else {
                currentSum += std::stoi(line);
            }

        } while(ok);
        caloriesPerElf.push_back(currentSum);
    }
    std::nth_element(caloriesPerElf.begin(), caloriesPerElf.begin() + 2, caloriesPerElf.end(),
                     std::greater<int>());
    std::cout << "current max: " << std::reduce(caloriesPerElf.begin(), caloriesPerElf.begin() + 3)
              << std::endl;
}
