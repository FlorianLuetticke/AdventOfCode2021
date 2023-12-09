#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <stdexcept>
#include <vector>
enum class PlayMove
{
    Rock     = 1,
    Paper    = 2,
    Scissors = 3
};
enum class PlayResult
{
    Loose = 0,
    Draw  = 3,
    Win   = 6
};
class StrategyGuide
{
public:
    StrategyGuide(std::ifstream& input)
    {
        while(!input.eof()) {
            std::string line;
            std::getline(input, line);
            if(line.size() < 3) {
                break;
            }
            std::map<char, PlayMove> elveMoves{
                {'A', PlayMove::Rock}, {'B', PlayMove::Paper}, {'C', PlayMove::Scissors}};
            std::map<char, PlayMove> myMoves{
                {'X', PlayMove::Rock}, {'Y', PlayMove::Paper}, {'Z', PlayMove::Scissors}};
            std::map<char, PlayResult> results{
                {'X', PlayResult::Loose}, {'Y', PlayResult::Draw}, {'Z', PlayResult::Win}};
            strategem.emplace_back(elveMoves.at(line[0]), myMoves.at(line[2]));
            strategem2.emplace_back(elveMoves.at(line[0]), results.at(line[2]));
        }
    }
    PlayMove getWinningMove(PlayMove in)
    {

        switch(in) {
            case PlayMove::Rock:
                return PlayMove::Paper;
            case PlayMove::Paper:
                return PlayMove::Scissors;
            case PlayMove::Scissors:
                return PlayMove::Rock;
        }
        throw std::logic_error("cnontrol reaches end of non-void function");
    }
    PlayMove getLoosingMove(PlayMove in)
    {
        switch(in) {
            case PlayMove::Rock:
                return PlayMove::Scissors;
            case PlayMove::Paper:
                return PlayMove::Rock;
            case PlayMove::Scissors:
                return PlayMove::Paper;
        }
        throw std::logic_error("cnontrol reaches end of non-void function");
    }

    std::vector<std::pair<PlayMove, PlayMove>>   strategem;
    std::vector<std::pair<PlayMove, PlayResult>> strategem2;
};
int main(int argc, char** argv)
{
    std::ifstream input("input.txt", std::ifstream::in);
    if(!input.is_open()) {
        std::cout << "Error opening\n";
        return -1;
    }
    StrategyGuide guide(input);

    int score = 0;

    for(auto [elf, mine] : guide.strategem) {

        score += int(mine);
        if(elf == mine) {
            score += 3;
        }

        if((mine == PlayMove::Rock && elf == PlayMove::Scissors) ||
           (mine == PlayMove::Paper && elf == PlayMove::Rock) ||
           (mine == PlayMove::Scissors && elf == PlayMove::Paper))
        {
            score += 6;
        }
    }
    std::cout << "Score " << score << std::endl;
    score = 0;

    for(auto [elf, result] : guide.strategem2) {
        switch(result) {
            case PlayResult::Loose:
                score += int(result) + int(guide.getLoosingMove(elf));
                break;
            case PlayResult::Draw:
                score += int(result) + int(elf);
                break;
            case PlayResult::Win:
                score += int(result) + int(guide.getWinningMove(elf));
                break;
        }
    }
    std::cout << "Score2 " << score << std::endl;
    return 0;
}
