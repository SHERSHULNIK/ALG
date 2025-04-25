#include <stdio.h>
#include <iostream>
#include <vector>
using std::string;
using std::cout;
using std::cin;

std::vector<int> buildBadCharTable(const std::string& pattern) { // Таблица плохого символа
    std::vector<int> badChar(256, -1);
    for (int i = 0; i < pattern.size(); ++i) {
        badChar[static_cast<unsigned char>(pattern[i])] = i;
    }
    return badChar;
}
int boyerMooreSearchFirst(const std::string text, const std::string pattern) // Первое вхождение
{
    int textSize = text.size();
    int patternSize = pattern.size();
    if (patternSize == 0) {
        return 0;
    }

    std::vector<int> badCharacterTable = buildBadCharTable(pattern);

    int pos = 0;
    while (pos <= textSize - patternSize)
    {
        int j = patternSize - 1;
        while (j >= 0 && pattern[j] == text[pos + j]) {
            --j;
        }
        if (j < 0) {
            return pos;
        }
        else {
            pos += std::max(1, j - badCharacterTable[static_cast<unsigned char>(text[pos + j])]);
        }
    }
    return -1;
}

std::vector<int> boyerMooreSearchAll(const std::string text, const std::string pattern, int from = 0, int to = -1) { // Все вхождения подстроки
    std::vector<int> result;
    int textSize = text.size();
    int patternSize = pattern.size();

    if (from > textSize) {
        return result;
    }
    if (from < 0) {
        from = textSize + from;
    }
    if (to < 0) {
        to = textSize + to;
    }
    if (to <= from) {
        return result;
    }

    int currentPos = from;
    int substrLength = to - from + 1;
    while (substrLength >= patternSize) {
        int foundPos = boyerMooreSearchFirst(text.substr(currentPos, substrLength), pattern);

        if (foundPos == -1) {
            break;
        }

        result.push_back(currentPos + foundPos);

        currentPos += foundPos + 1;
        substrLength = to - currentPos + 1;
    }
    return result;
}
int main() // мэйн >.<
{
    std::string text = { "std::move_iterator is an iterator adaptor which behaves exactly like the underlying iterator" };
    std::string pattern = { "tor" };

    int resultBoyerMooreSearch = boyerMooreSearchFirst(text, pattern);
    std::vector<int> resultBoyerMooreSearchAll = boyerMooreSearchAll(text, pattern, -17, -12);

    std::cout << "text: " << text << std::endl;
    std::cout << "pattern: " << pattern << std::endl;
    std::cout << resultBoyerMooreSearch << std::endl;
    for (const auto& elem : resultBoyerMooreSearchAll) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}