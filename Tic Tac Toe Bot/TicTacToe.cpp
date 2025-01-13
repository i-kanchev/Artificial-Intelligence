#include <iostream>
#include <vector>

const int BOARD_SIZE = 3;
const int MAX_DEPTH = 10;
const char PLAYER_SYMBOL = 'X';
const char COMPLUTER_SYMBOL = 'O';

// alpha - best option of maximizer
// beta - best option of minimizer
std::pair<int, int> maximizer(const int turnsCount, std::vector<int>& board, const int alpha, const int beta);
std::pair<int, int> minimizer(const int turnsCount, std::vector<int>& board, const int alpha, const int beta);

void printBoard(const std::vector<int> board) {
	std::cout << std::endl;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i * BOARD_SIZE + j] == 1) {
				std::cout << ' ' << PLAYER_SYMBOL;
			}
			else if (board[i * BOARD_SIZE + j] == -1) {
				std::cout << ' ' << COMPLUTER_SYMBOL;
			}
			else {
				std::cout << "  ";
			}
			if (j != BOARD_SIZE - 1) {
				std::cout << " |";
			}
		}

		std::cout << std::endl;

		if (i != BOARD_SIZE - 1) {
			std::cout << " - + - + -" << std::endl;
		}
	}

	std::cout << std::endl;
}

// 0 - none/still in progress, positive - player wins, negative - computer wins
int isGameOver(const int turnsCount, const std::vector<int> board) {
	if (turnsCount < 5) {
		return 0;
	}

	if ((board[0] + board[1] + board[2] == 3)
		|| (board[3] + board[4] + board[5] == 3)
		|| (board[6] + board[7] + board[8] == 3)
		|| (board[0] + board[3] + board[6] == 3)
		|| (board[1] + board[4] + board[7] == 3)
		|| (board[2] + board[5] + board[8] == 3)
		|| (board[0] + board[4] + board[8] == 3)
		|| (board[2] + board[4] + board[6] == 3)) {
		return MAX_DEPTH - turnsCount;
	}

	if ((board[0] + board[1] + board[2] == -3)
		|| (board[3] + board[4] + board[5] == -3)
		|| (board[6] + board[7] + board[8] == -3)
		|| (board[0] + board[3] + board[6] == -3)
		|| (board[1] + board[4] + board[7] == -3)
		|| (board[2] + board[5] + board[8] == -3)
		|| (board[0] + board[4] + board[8] == -3)
		|| (board[2] + board[4] + board[6] == -3)) {
		return -MAX_DEPTH + turnsCount;
	}

	return 0;
}

// value index
std::pair<int, int> maximizer(const int turnsCount, std::vector<int>& board, int alpha, int beta) {
	int state = isGameOver(turnsCount, board);

	if (turnsCount == 9 || state != 0) {
		return { state, -1 };
	}
	
	int maxValue = INT_MIN;
	int index = -1;

	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		if (board[i] != 0) {
			continue;
		}

		if (maxValue > beta) {
			return { maxValue, index };
		}

		board[i] = 1;

		std::pair<int, int> curr = minimizer(turnsCount + 1, board, alpha, beta);

		if (curr.first > maxValue) {
			maxValue = curr.first;
			index = i;

			alpha = curr.first;
		}

		board[i] = 0;
	}

	return { maxValue, index };
}

// value index
std::pair<int, int> minimizer(const int turnsCount, std::vector<int>& board, int alpha, int beta) {
	int state = isGameOver(turnsCount, board);
	
	if (turnsCount == 9 || state != 0) {
		return { state, -1 };
	}
	
	int minValue = INT_MAX;
	int index = -1;

	for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
		if (board[i] != 0) {
			continue;
		}

		if (minValue < alpha) {
			return { minValue, index };
		}

		board[i] = -1;

		std::pair<int, int> curr = maximizer(turnsCount + 1, board, alpha, beta);

		if (curr.first < minValue) {
			minValue = curr.first;
			index = i;

			beta = curr.first;
		}

		board[i] = 0;
	}

	return { minValue, index };
}

int main()
{
	bool isPlayersTurn = true;
	int turnsCount = 0;
	int gameState = 0;
	
	// 0 - neutral, 1 - player, -1 - computer
	std::vector<int> board(BOARD_SIZE * BOARD_SIZE);

	while (turnsCount < 9 && gameState == 0) {
		printBoard(board);

		int index;

		if (isPlayersTurn) {
			std::cout << "Player turn" << std::endl;
			std::cout << "Enter coordinates: ";

			int x, y;
			std::cin >> x >> y;

			index = (x - 1) * BOARD_SIZE + (y - 1);

			board[index] = 1;
		}
		else {
			std::cout << "Computer turn" << std::endl;

			index = minimizer(turnsCount, board, INT_MIN, INT_MAX).second;

			board[index] = -1;
		}

		isPlayersTurn = !isPlayersTurn;
		turnsCount++;

		gameState = isGameOver(turnsCount, board);
	}
	
	printBoard(board);

	if (gameState >= 1) {
		std::cout << "Player wins" << std::endl;
	}
	else if (gameState <= -1) {
		std::cout << "Computer wins" << std::endl;
	}
	else {
		std::cout << "Draw" << std::endl;
	}

	return 0;
}
