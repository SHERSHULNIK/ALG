#include <stdio.h>
#include <iostream>
#include <vector>
using std::string;
using std::cout;
using std::cin;

std::vector<int> buildBadCharTable(const std::string& pattern) { // Таблица плохого символа
    int n = pattern.size();
    std::vector<int> badChar(256, n);
    for (int i = 0; i < pattern.size() - 1; ++i) {
        badChar[static_cast<unsigned char>(pattern[i])] = n - i - 1;
        std::cout << pattern[i] << ' ' << badChar[static_cast<unsigned char>(pattern[i])] << '\n';
    }
    std::cout << "=========\n";
    for (int i = 0; i < pattern.size(); ++i) {
        std::cout << pattern[i] << ' ' << badChar[static_cast<unsigned char>(pattern[i])] << '\n';
    }
    std::cout << "---------\n";
    return badChar;
}

int boyerMooreSearchFirst(const std::string text, const std::string pattern) // Первое вхождение
{
    int pos = 0;
    int textSize = text.size();
    int patternSize = pattern.size();
    if (patternSize == 0) { 
        return 0;
    }
    std::vector<int> badCharacterTable = buildBadCharTable(pattern);
    std::cout << text << "\n"; // Вывод всего текста для реализации сдвига
    while (pos <= textSize - patternSize) // Границы для выполнения цикла 
    {
        for (int i = 0; i < pos; i++) { // Для реалиизации сдвига -->
            std::cout << ' ';       
        }
        std::cout << pattern << "\n"; // <--        
        int j = patternSize - 1; 
        while (j >= 0 && pattern[j] == text[pos + j]) {
            --j;
        }
        if (j < 0) {
            return pos;
        }
        else {
            //i = i + T[S[i]]
            //pos = pos + badCharacterTable[text[pos]]
            std::cout << j << ' ' << pos << ' ' << text[pos + patternSize - 1] << ' ' << badCharacterTable[static_cast<unsigned char>(text[pos + patternSize - 1])] << "\n"; // Вывод всего текста для реализации сдвига
            pos += (badCharacterTable[static_cast<unsigned char>(text[pos + patternSize - 1])]);
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
    std::string pattern = { "trallolo" };
    std::string text = { "pij pij ololollo voditel' nlo" };

    int resultBoyerMooreSearch = boyerMooreSearchFirst(text, pattern);
    std::vector<int> resultBoyerMooreSearchAll = boyerMooreSearchAll(text, pattern);

    std::cout << "pattern: " << pattern << std::endl;
    std::cout << "text: " << text << std::endl;
    std::cout << resultBoyerMooreSearch << std::endl;
    for (const auto& elem : resultBoyerMooreSearchAll) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}