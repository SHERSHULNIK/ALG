#include <stdio.h>
#include <iostream>
#include <vector>
using std::string;
using std::cout;
using std::cin;

std::vector<int> buildBadCharTable(const std::string& pattern) {
    std::vector<int> badChar(256, -1);
    for (int i = 0; i < pattern.size(); ++i) {
        badChar[static_cast<unsigned char>(pattern[i])] = i;
    }
    return badChar;
}
