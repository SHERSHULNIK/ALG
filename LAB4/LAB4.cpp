﻿#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <chrono>
#include <iomanip>

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

void buildHeap(std::vector<int>& vector, int heapSize, int i) {
    int root = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heapSize && vector[left] > vector[root]) {
        root = left;
    }

    if (right < heapSize && vector[right] > vector[root]) {
        root = right;
    }

    if (root != i) {
        std::swap(vector[i], vector[root]);
        buildHeap(vector, heapSize, root);
    }
}

void heapSort(std::vector<int>& vector) {
    int vectorSize = vector.size();

    for (int i = vectorSize / 2 - 1; i >= 0; i--) {
        buildHeap(vector, vectorSize, i);
    }

    for (int i = vectorSize - 1; i > 0; i--) {
        std::swap(vector[0], vector[i]);
        buildHeap(vector, i, 0);
    }
}


int main() {
    std::vector<std::string> filenames = {
    "10000_numbers_in_range_10.txt",
    "10000_numbers_in_range_1000.txt",
    "10000_numbers_in_range_100000.txt",
    "100000_numbers_in_range_10.txt",
    "100000_numbers_in_range_1000.txt",
    "100000_numbers_in_range_100000.txt",
    "1000000_numbers_in_range_10.txt",
    "1000000_numbers_in_range_1000.txt",
    "1000000_numbers_in_range_100000.txt"
    };

    const int runs = 3;
    std::ofstream resultsFile("results.txt");
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
            heapSort(data);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            times.push_back(duration.count());

            if (!isSorted(data)) {
                allCorrect = false;
            }
        }

        double average = (times[0] + times[1] + times[2]) / runs;
        resultsFile << filename << ", heapSort, " << average << ", " << (allCorrect ? "Yes" : "No") << "\n";
    }

    resultsFile.close();
    std::cout << "All tests completed!" << std::endl;

    return 0;
}