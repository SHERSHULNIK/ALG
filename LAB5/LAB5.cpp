#include <algorithm>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <iostream>


bool isSorted(const std::vector<int>&vector) {
    return std::is_sorted(vector.begin(), vector.end());
}

std::vector<int> loadFile(const std::string& filename) {
    std::ifstream in(filename);
    std::vector<int> vector;
    int number;
    while (in >> number) {
        vector.push_back(number);
    }
    return vector;
}

void quickSort(std::vector<int>& vector, int leftBound, int rightBound) {
    if (leftBound >= rightBound) {
        return;
    }

    int i = leftBound;
    int j = rightBound;
    int central = vector[(leftBound + rightBound) / 2];

    while (i <= j) {
        while (vector[i] < central) {
            i++;
        }

        while (vector[j] > central) {
            j--;
        }

        if (i <= j) {
            std::swap(vector[i], vector[j]);
            i++;
            j--;
        }
    }

    quickSort(vector, leftBound, j);
    quickSort(vector, i, rightBound);
}

int main()
{
    std::vector<std::string> filenames = {
        "ARRAY1.txt",
        "ARRAY2.txt",
        "ARRAY3.txt",
        "ARRAY4.txt",
        "ARRAY5.txt",
        "ARRAY6.txt",
        "ARRAY7.txt",
        "ARRAY8.txt",
        "ARRAY9.txt"
    };

    const int runs = 3;
    std::ofstream resultsFile("resultslab5.txt");
    resultsFile << "Filename, Algorithm, Average (s), Sorted Correctly\n";
    resultsFile << std::fixed << std::setprecision(6);

    for (const auto& filename : filenames) {
        std::cout << "Testing file: " << filename << std::endl;
        auto originalData = loadFile(filename);

        std::vector<double> times;
        bool allCorrect = true;

        for (int i = 0; i < runs; ++i) {
            auto data = originalData;
            auto start = std::chrono::high_resolution_clock::now();
            quickSort(data, 0, data.size() - 1);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            times.push_back(duration.count());

            if (!isSorted(data)) {
                allCorrect = false;
            }
        }

        double average = (times[0] + times[1] + times[2]) / runs;
        resultsFile << filename << ", quickSort, " << average << ", " << (allCorrect ? "Yes" : "No") << "\n";
    }

    resultsFile.close();
    std::cout << "All tests completed!" << std::endl;

    return 0;
}