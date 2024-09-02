#pragma once
#include <vector>
#include <string>


static class Lib
{
public:
    static std::vector<std::string> SplitString(std::string stringToSplit, char delim = ' ');
};
