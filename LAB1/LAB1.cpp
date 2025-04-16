#include <stdio.h>
#include <iostream>
#include <time.h>
#include <ctime>
using std::cout;
using std::cin;

void OutputMasPtr(int* a, int n)
{
	for (int* p = a; p < a + n; p++)
		printf("%5d", *p);
}
void RandMatrDin(int** matr, int row, int col, int from, int to)
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (i == j) {
				matr[i][j] = 0;
			}
			else {
				matr[i][j] = rand() % (to + 1 - from) + from;
			}
		}
	}
}
void OutputMatrDin(int** matr, int row, int col)
{
	cout << "\n";
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("%4d", matr[i][j]);
		}
		cout << "\n";
	}
}
void swap(int& a, int& b)
{
	int c;
	c = a;
	a = b;
	b = c;
}
bool nextWay(int* way, int size) //Функция перестановки неповторяющихся чисел
{
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
	while (l < r)
	{
		swap(way[l], way[r]);
		l++;
		r--;
	}
	return true;
}
int SumMatr(int** matr, int size, int* way) // Функция суммы по пути
{
	int sum = 0;
	for (int i = 1; i < size; i++) {
		sum += matr[way[i - 1] - 1][way[i] - 1];
	}
	sum += matr[way[size - 1] - 1][way[0] - 1];
	return sum;
}
void Exact(int** matr, int size, int* way, int* waymin, int& MinSum, int& MaxSum, bool debug = false) // Точный алгоритм
{
	int sum;
	sum = SumMatr(matr, size, way);
	if (debug)
	{
		OutputMasPtr(way, size);
		cout << ", sum = " << sum << "\n";
	}
	MinSum = sum;
	MaxSum = sum;
	sum = 0;
	while (nextWay(way, size)) {
		sum = SumMatr(matr, size, way);
		if (debug)
		{
			OutputMasPtr(way, size);
			cout << ", sum = " << sum << "\n";
		}
		if (sum < MinSum) {
			MinSum = sum;
			for (int i = 0; i < size; i++) {
				waymin[i] = way[i];
			}
		}
		if (sum > MaxSum) {
			MaxSum = sum;
		}
		sum = 0;
	}
}
bool check(int* way, int size, int n) // проверка чтобы не было цикла
{
	for (int i = 0; i < size; i++)
	{
		if (n == way[i] - 1)
			return false;
	}
	return true;
}
int findMinInRow(int** matr, int size, int row, int* way) // нахождение минимального числа в строке
{
	int min_col = 0;
	while ((matr[row][min_col] == 0) && (min_col < size) || (!check(way, size, min_col))) {
		min_col++;
	}
	for (int i = min_col; i < size; i++) {
		if ((matr[row][i] < matr[row][min_col]) && (matr[row][i] != 0) && (check(way, size, i))) {
			min_col = i;
		}
	}
	return min_col;
}
void null(int** matr, int size, int row, int col) // зануление в эвристике
{
	for (int i = 0; i < size; i++) {
		matr[row][i] = 0;
	}
	for (int j = 0; j < size; j++) {
		matr[j][col] = 0;
	}
	matr[col][row] = 0;
}
int Heuristics(int** matr, int size, int* way, bool debug = false) // Жадный алгоритм
{
	int row = way[0] - 1, col, sum = 0;
	for (int i = 1; i < size; i++) {
		col = findMinInRow(matr, size, row, way);
		way[i] = col + 1;
		sum += matr[row][col];
		null(matr, size, row, col);
		row = col;
		if (debug) {
			OutputMatrDin(matr, size, size);
			puts("");
		}
	}
	sum += matr[row][way[0] - 1];
	return sum;
}

void makeReport(int size, int first)
{
	cout << "\nРазмер - " << size;
	cout << "\nНачало - " << first;
	clock_t start, stop, start1, stop1; double T, T1;
	int MinSumExact = 0, SumHeuristics = 0, MaxSumExact = 0;
	int* way = new int[size];
	int* waymin = new int[size];
	int** matr = new int* [size];
	for (int i = 0; i < size; i++) {
		matr[i] = new int[size];
	}
	RandMatrDin(matr, size, size, 1, 20);
	//	InputMatrDin(matr, razmer, razmer);
	OutputMatrDin(matr, size, size);
	way[0] = first;
	waymin[0] = first;

	start = clock();
	for (int i = 1, m = 1; i < size; i++, m++)
	{
		if (i == first) {
			m++;
		}
		way[i] = m;
		waymin[i] = m;
	}
	Exact(matr, size, way, waymin, MinSumExact, MaxSumExact);
	stop = clock();

	cout << "\nМинимальная сумма точного алгоритма - " << MinSumExact;
	cout << "\n Путь - ";
	OutputMasPtr(waymin, size);
	cout << "\n\nМаксимальная сумма точного алгоритма - " << MaxSumExact;
	puts("");

	start1 = clock();
	for (int i = 1; i < size; i++) {
		way[i] = 0;
	}
	SumHeuristics = Heuristics(matr, size, way);
	stop1 = clock();

	cout << "\nСумма эвристики - " << SumHeuristics;
	cout << "\n Путь - ";
	OutputMasPtr(way, size);

	if (MaxSumExact - MinSumExact == 0) {
		cout << "\n\nТочность выполнения эвристики - 100";
	}
	else {
		double percent = (1 - (1.0 * (SumHeuristics - MinSumExact) / (MaxSumExact - MinSumExact))) * 100;
		cout << "\n\nТочность выполнения эвристики - " << percent;
	}

	T = (double)(stop - start) / CLOCKS_PER_SEC;
	T1 = (double)(stop1 - start1) / CLOCKS_PER_SEC;
	puts("");
	cout << "\nВремя выполнения точного алгоритма - " << T;
	cout << "\nВремя выполнения эвристики - " << T1;
	puts("\n\n");
	for (int i = 0; i < size; i++) {
		delete[] matr[i];
	}
	delete[] matr;
	delete[] way;
	delete[] waymin;
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