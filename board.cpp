#include "board.h"
#include <stdexcept>
#include <algorithm>
#include <numeric>

Board::Board(int size) : boardSize(size) {
	if (boardSize <= 0) {
		throw std::invalid_argument("Board size must be positive.");
	}
}
