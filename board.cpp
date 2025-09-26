#include "board.h"
#include <stdexcept>
#include <algorithm>
#include <numeric>

Board::Board(int size) : boardSize(size) {
	if (boardSize <= 0) {
		throw std::invalid_argument("Board size must be positive.");
	}
}

void Board::mark_blocked_all_sides(std::vector<unsigned char>& blocked, int row, int col) const {
	for (int dr = -1; dr <= 1; ++dr) {
		long neighborRow = static_cast<long>(row) + dr;
		if (neighborRow < 0 || neighborRow >= boardSize) continue;

		for (int dc = -1; dc <= 1; ++dc) {
			long neighborCol = static_cast<long>(col) + dc;
			if (neighborCol < 0 || neighborCol >= boardSize) continue;

			blocked[static_cast <size_t>(neighborRow * boardSize + neighborCol)] = 1;
		}
	}
}

void Board::mark_blocked_vertical_horizontal(std::vector<unsigned char>& blocked, int row, int col) const {
	auto mark = [&](long neighborRow, long neighborCol) {
		if (neighborRow >= 0 && neighborRow < boardSize && neighborCol >= 0 && neighborCol < boardSize) {
			blocked[static_cast<size_t>(neighborRow * boardSize + neighborCol)] = 1;
		}
		};
	mark(row, col);
	mark(row -1, col);
	mark(row + 1, col);
	mark(row, col -1);
	mark(row, col +1);
}

std::size_t Board::free_zone_size(const std::vector<std::pair<int, int>>& selected, int adj) const {
	std::vector<unsigned char> blocked(boardSize * boardSize, 0);

	for (const auto& selectedCell : selected) {
		const int row = selectedCell.first;
		const int col = selectedCell.second;
		if (row < 0 || row >= boardSize || col < 0 || col >= boardSize) {
			throw std::out_of_range("Selected cell is out of board bounds");
		}

		if (adj == ALL_SIDES) {
			mark_blocked_all_sides(blocked, row, col);
		}
		else {
			mark_blocked_vertical_horizontal(blocked, row, col);
		}
	}

	return static_cast<std::size_t>(std::count(blocked.begin(), blocked.end(), 0));
}