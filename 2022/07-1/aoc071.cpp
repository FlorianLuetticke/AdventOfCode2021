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
class DirNode
{

public:
    DirNode(std::string _name, bool _isFile, size_t _size,
            std::weak_ptr<DirNode> _parent = std::shared_ptr<DirNode>())
    {
        name   = _name;
        parent = _parent;
        isFile = _isFile;
        size   = _size;
    }
    std::weak_ptr<DirNode>                parent;
    std::string                           name;
    size_t                                size = 0;
    std::vector<std::shared_ptr<DirNode>> subNodes;
    bool                                  isFile = true;
    static std::shared_ptr<DirNode>       addDir(std::shared_ptr<DirNode> parent, std::string name)
    {
        if(parent->isFile) {
            throw std::logic_error("tis a file");
        }
        parent->subNodes.emplace_back(std::make_shared<DirNode>(name, false, 0, parent));
        return parent->subNodes.back();
    }
    static void addFile(std::shared_ptr<DirNode> parent, std::string name, size_t size)
    {
        if(parent->isFile) {
            throw std::logic_error("tis a file");
        }
        parent->subNodes.emplace_back(std::make_shared<DirNode>(name, true, size, parent));
    }
    void calcSize()
    {
        if(isFile) {
            return;
        }
        size_t sum = 0;
        for(auto& node : subNodes) {
            node->calcSize();
            sum += node->size;
        }
        size = sum;
    }

    size_t smallDirCount(size_t indent)
    {
        if(isFile) {
            return 0;
        }
        size_t sum = 0;
        for(auto& node : subNodes) {
            sum += node->smallDirCount(indent + 1);
        }
        for(auto i = 0ul; i < indent; ++i) {
            std::cout << "    ";
        }
        std::cout << "Dir " << name << " has " << sum << " small subDirs";

        if(size <= 100000) {
            std::cout << " and is itself small";
            sum++;
        }
        std::cout << std::endl;
        return sum;
    }

    size_t smallDirSize(size_t indent)
    {
        if(isFile) {
            return 0;
        }
        size_t sum = 0;
        for(auto& node : subNodes) {
            sum += node->smallDirSize(indent + 1);
        }
        for(auto i = 0ul; i < indent; ++i) {
            // std::cout << "    ";
        }
        // std::cout << "Dir " << name << " has " << sum << " small subDirs";

        if(size <= 100000) {
            // std::cout << " and is itself small";
            sum += size;
        }
        std::cout << std::endl;
        return sum;
    }
    void getDirSizes(std::vector<size_t>& dirSizes, size_t minSize)
    {
        if(isFile) {
            return;
        }
        if(size >= minSize) {
            dirSizes.push_back(size);
        }

        for(auto& node : subNodes) {
            node->getDirSizes(dirSizes, minSize);
        }
    }

    void print(size_t indent)
    {
        for(auto i = 0ul; i < indent; ++i) {
            std::cout << "    ";
        }
        if(isFile) {
            std::cout << "file " << name << " " << size << "\n";
            return;
        }
        else {

            std::cout << "DIR  " << name << " " << size;
            if(size <= 100000) {
                std::cout << " SMALL!";
            }
            std::cout << "\n";
        }

        for(auto& node : subNodes) {
            node->print(indent + 1);
        }
    }

    std::shared_ptr<DirNode> findNode(std::string name)
    {
        for(auto& node : subNodes) {
            if(node->name == name)
                return node;
        }
        return nullptr;
    }
    static std::shared_ptr<DirNode> getDir(std::shared_ptr<DirNode> parent, std::string name)
    {
        auto ret = parent->findNode(name);
        if(ret != nullptr) {
            return ret;
        }
        return addDir(parent, name);
    }
};

int main(int argc, char** argv)
{
    std::ifstream input("input.txt", std::ifstream::in);
    if(!input.is_open()) {
        std::cout << "Error opening\n";
        return -1;
    }

    auto        root    = std::make_shared<DirNode>("", false, 0);
    auto        current = root;
    std::string line;
    std::getline(input, line);
    while(!input.eof()) {
        std::cout << "work on " << line << "\n";

        auto tokens = csvToStringVec(line, " ");
        if(tokens.at(0) == "$") {
            std::cout << "Token " << tokens.at(1) << " " << std::boolalpha << (tokens.at(1) == "cd")
                      << std::endl;

            if(tokens.at(1) == "cd") {
                std::cout << "found command cd " << line << "\n";
                if(tokens.at(2) == "/") {
                    current = root;
                }
                else if(tokens.at(2) == "..") {
                    current = current->parent.lock();
                }
                else {
                    std::cout << "do getDir" << std::endl;
                    current = DirNode::getDir(current, tokens.at(2));
                }
            }
            else if(tokens.at(1) == "ls") {
                std::cout << "found command ls " << line << "\n";
                while(true) {
                    line.clear();
                    std::getline(input, line);
                    if(input.eof() && line.size() == 0) {
                        break;
                    }
                    if(line.at(0) == '$') {
                        break;
                    }

                    auto [lhs, rhs] = split(line, " ");
                    if(lhs != "dir") {
                        current->addFile(current, rhs, std::stoul(lhs));
                    }
                }

                continue; // skip getline
            }
            else {
                std::cout << "  bad " << line << " is bad \n";
                throw std::logic_error("bad command");
            }
            std::getline(input, line);
        }
    }
    root->calcSize();
    root->print(0);
    auto count = root->smallDirCount(0);
    auto sum   = root->smallDirSize(0);
    std::cout << "The count is " << sum << std::endl;

    size_t discSize     = 70000000;
    size_t usedSize     = root->size;
    size_t freeSize     = discSize - usedSize;
    size_t needToDelete = 30000000 - freeSize;
    std::cout << "used size " << usedSize << " bytes\n";
    std::cout << "need to delete " << needToDelete << " bytes\n";
    std::vector<size_t> directorySizes;
    root->getDirSizes(directorySizes, needToDelete);
    std::sort(directorySizes.begin(), directorySizes.end());

    std::cout << "delete " << directorySizes.front() << "\n";

    return 0;
}
