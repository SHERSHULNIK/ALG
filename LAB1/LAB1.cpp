#include <iostream>
#include <vector>
#include <ctime>
#include <cstdio>
#include <algorithm>
using std::cout;
using std::cin;

void OutputMasPtr(const std::vector<int>&a)
{
	for (const auto& elem : a) {
		printf("%5d", elem);
	}
}
void RandMatrDin(std::vector<std::vector<int>>& matr, int from, int to)
{
	int size = matr.size();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == j) {
				matr[i][j] = 0;
			}
			else {
				matr[i][j] = rand() % (to + 1 - from) + from;
			}
		}
	}
}
void OutputMatrDin(const std::vector<std::vector<int>>& matr)
{
	cout << "\n";
	for (const auto& row : matr) {
		for (const auto& elem : row) {
			printf("%4d", elem);
		}
		std::cout << "\n";
	}
}
void swap(int& a, int& b) { // SWAPчик
	std::swap(a, b);
}
void null(std::vector<std::vector<int>>& matr, int row, int col) { // Зануление определенных данных или переменных
	int size = matr.size();
	for (int i = 0; i < size; i++) {
		matr[row][i] = 0;
	}
	for (int j = 0; j < size; j++) {
		matr[j][col] = 0;
	}
	matr[col][row] = 0;
}
bool check(const std::vector<int>& way, int n) { // Проверка на отсутсвие зацикливания
	for (int elem : way) {
		if (n == elem - 1)
			return false;
	}
	return true;
}
int findMinInRow(const std::vector<std::vector<int>>& matr, int row, const std::vector<int>& way) { // Найти минимальное в ряду
	int size = matr.size();
	int min_col = 0;
	while ((min_col < size) &&
		((matr[row][min_col] == 0) || (!check(way, min_col)))) {
		min_col++;
	}
	for (int i = min_col; i < size; i++) {
		if ((matr[row][i] < matr[row][min_col]) && (matr[row][i] != 0) && (check(way, i))) {
			min_col = i;
		}
	}
	return min_col;
}

bool nextWay(std::vector<int>& way) { // Функция перестановки
	int size = way.size();
	int j = size - 2;
	while (j != 0 && way[j] >= way[j + 1]) {
		j--;
	}
	if (j == 0) {
		return false;
	}
	int k = size - 1;
	while (way[j] >= way[k] && k != 0) {
		k--;
	}
	swap(way[j], way[k]);
	int l = j + 1, r = size - 1;
	while (l < r) {
		swap(way[l], way[r]);
		l++;
		r--;
	}
	return true;
}

int SumMatr(const std::vector<std::vector<int>>& matr, const std::vector<int>& way) { // Функция суммы по пути
	int sum = 0;
	int size = way.size();
	for (int i = 1; i < size; i++) {
		sum += matr[way[i - 1] - 1][way[i] - 1];
	}
	sum += matr[way[size - 1] - 1][way[0] - 1];
	return sum;
}
void Exact(const std::vector<std::vector<int>>& matr, std::vector<int>& way, // Точный алгоритм
	std::vector<int>& waymin, int& MinSum, int& MaxSum, bool debug = false) {
	int sum = SumMatr(matr, way);
	if (debug) {
		OutputMasPtr(way);
		std::cout << ", sum = " << sum << "\n";
	}
	MinSum = sum;
	MaxSum = sum;
	while (nextWay(way)) {
		sum = SumMatr(matr, way);
		if (debug) {
			OutputMasPtr(way);
			std::cout << ", sum = " << sum << "\n";
		}
		if (sum < MinSum) {
			MinSum = sum;
			waymin = way;
		}
		if (sum > MaxSum) {
			MaxSum = sum;
		}
	}
}	



int Heuristics(std::vector<std::vector<int>>& matr, std::vector<int>& way, bool debug = false) { // Жадина
	int size = matr.size();
	int row = way[0] - 1, col, sum = 0;
	for (int i = 1; i < size; i++) {
		col = findMinInRow(matr, row, way);
		way[i] = col + 1;
		sum += matr[row][col];
		null(matr, row, col);
		row = col;
		if (debug) {
			OutputMatrDin(matr);
			puts("");
		}
	}
	sum += matr[row][way[0] - 1];
	return sum;
}	


void makeReport(int size, int first) { // Заявление на готовую лабу
	std::cout << "\nРазмер - " << size;
	std::cout << "\nНачало - " << first;
	clock_t start, stop, start1, stop1;
	double T, T1;
	int MinSumExact = 0, SumHeuristics = 0, MaxSumExact = 0;

	std::vector<int> way(size);
	std::vector<int> waymin(size);
	std::vector<std::vector<int>> matr(size, std::vector<int>(size));

	RandMatrDin(matr, 1, 20);
	OutputMatrDin(matr);

	way[0] = first;
	waymin[0] = first;

	start = clock();
	for (int i = 1, m = 1; i < size; i++, m++) {
		if (i == first) {
			m++;
		}
		way[i] = m;
		waymin[i] = m;
	}
	Exact(matr, way, waymin, MinSumExact, MaxSumExact);
	stop = clock();

	std::cout << "\nМинимальная сумма точного алгоритма - " << MinSumExact;
	std::cout << "\n Путь - ";
	OutputMasPtr(waymin);
	std::cout << "\n\nМаксимальная сумма точного алгоритма - " << MaxSumExact;
	puts("");

	start1 = clock();
	std::fill(way.begin() + 1, way.end(), 0);
	SumHeuristics = Heuristics(matr, way);
	stop1 = clock();

	std::cout << "\nСумма эвристики - " << SumHeuristics;
	std::cout << "\n Путь - ";
	OutputMasPtr(way);

	if (MaxSumExact - MinSumExact == 0) {
		std::cout << "\n\nТочность выполнения эвристики - 100";
	}
	else {
		double percent = (1 - (1.0 * (SumHeuristics - MinSumExact) / (MaxSumExact - MinSumExact))) * 100;
		std::cout << "\n\nТочность выполнения эвристики - " << percent;
	}

	T = (double)(stop - start) / CLOCKS_PER_SEC;
	T1 = (double)(stop1 - start1) / CLOCKS_PER_SEC;
	puts("");
	std::cout << "\nВремя выполнения точного алгоритма - " << T;
	std::cout << "\nВремя выполнения эвристики - " << T1;
	puts("\n\n");
}
int main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(0));

	for (int i = 1; i < 5; i++) {
		makeReport(4, i);
	}
	for (int i = 1; i < 5; i++) {
		makeReport(6, i);
	}
	for (int i = 1; i < 5; i++) {
		makeReport(8, i);
	}
	for (int i = 1; i < 5; i++) {
		makeReport(10, i);
	}
	return 0;
}