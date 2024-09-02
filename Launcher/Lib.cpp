#include "Lib.h"

std::vector<std::string> Lib::SplitString(std::string stringToSplit, char delim) {
    std::vector<std::string> words;
    size_t start = 0;
    size_t end;

    while ((end = stringToSplit.find(delim, start)) != std::string::npos) {
        words.push_back(stringToSplit.substr(start, end - start));
        start = end + 1;
    }

    words.push_back(stringToSplit.substr(start));
    return words;
}