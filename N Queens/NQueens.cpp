#include <iostream>
#include <vector>
#include <time.h>
#include <chrono>
#include <ctime>
#include <random>
#include <map>

void place(const int n, std::vector<int>& queens, std::vector<int>& rows, std::vector<int>& negDiags, std::vector<int>& posDiags) {
	queens.resize(n);
	rows.resize(n);
	negDiags.resize(2 * n - 1);
	posDiags.resize(2 * n - 1);
	
	std::vector<int> positions(n);

	for (int i = 0; i < n; i++) {
		positions[i] = i;
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(positions.begin(), positions.end(), g);

	for (int i = 0; i < n; i++) {
		int row = i;
		int col = positions[i];

		queens[col] = row;
		rows[row]++;
		negDiags[col - row + n - 1]++;
		posDiags[col + row]++;
	}
}

int maxConflictsCol(const int n, const std::vector<int>& queens, const std::vector<int>& rows,
	const std::vector<int>& negDiags, const std::vector<int>& posDiags) {

	int maxConf = 0;
	std::vector<int> colsMaxConf;

	for (int i = 0; i < n; i++) {
		int col = i;
		int row = queens[col];

		// excluding the queen from the 3 vectors
		int conflicts = rows[row] + negDiags[col - row + n - 1] + posDiags[col + row] - 3;

		if (conflicts == maxConf) {
			colsMaxConf.push_back(col);
		}
		else if (conflicts > maxConf) {
			maxConf = conflicts;
			colsMaxConf.clear();
			colsMaxConf.push_back(col);
		}
	}

	if (maxConf == 0)
	{
		return -1;
	}

	int rnd = rand() % colsMaxConf.size();

	return colsMaxConf[rnd];
}

int minConflictsRow(const int col, const int n, const std::vector<int>& queens, const std::vector<int>& rows,
	const std::vector<int>& negDiags, const std::vector<int>& posDiags) {
	
	int minConf = INT_MAX;
	std::vector<int> rowsMinConf;

	for (int i = 0; i < n; i++) {
		int row = i;

		int conflicts = rows[row] + negDiags[col - row + n - 1] + posDiags[col + row];

		// if there is a queen there, we need to remove it from the 3 vectors
		if (queens[col] == row) {
			conflicts -= 3;
		}

		if (conflicts == minConf) {
			rowsMinConf.push_back(row);
		}
		else if (conflicts < minConf) {
			minConf = conflicts;
			rowsMinConf.clear();
			rowsMinConf.push_back(row);
		}
	}

	int rnd = rand() % rowsMinConf.size();

	return rowsMinConf[rnd];
}

void print(const int n, const std::vector<int>& queens) {	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			queens[j] == i ? std::cout << "*" : std::cout << "-";
		}
		std::cout << std::endl;
	}
}

int main()
{
	srand((unsigned int)time(NULL));

	int n;
	
	std::cin >> n;

	if (n == 2 || n == 3) {
		std::cout << -1 << std::endl;
		
		return 0;
	}

	auto start = std::chrono::high_resolution_clock::now();

	std::vector<int> queens;
	std::vector<int> rows;
	std::vector<int> negDiags;
	std::vector<int> posDiags;
	
	place(n, queens, rows, negDiags, posDiags);

	int cnt = 0;

	while (true) {
		int col = maxConflictsCol(n, queens, rows, negDiags, posDiags);

		if (col == -1) {
			break;
		}

		int row = minConflictsRow(col, n, queens, rows, negDiags, posDiags);

		int currRow = queens[col];
		rows[currRow]--;
		negDiags[col - currRow + n - 1]--;
		posDiags[col + currRow]--;

		queens[col] = row;
		rows[row]++;
		negDiags[col - row + n - 1]++;
		posDiags[col + row]++;

		// reset if loop
		if (cnt++ > 2 * n * sqrt(n)) {
			cnt = 0;

			queens.clear();
			rows.clear();
			negDiags.clear();
			posDiags.clear();

			place(n, queens, rows, negDiags, posDiags);
		}
	}

	auto finish = std::chrono::high_resolution_clock::now();

	if (n <= 100) {
		print(n, queens);
	}

	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
	std::cout << static_cast<double>(microseconds.count()) / 1000000 << std::endl;

	return 0;
}
