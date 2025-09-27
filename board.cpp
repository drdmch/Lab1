#include "board.h"
#include "randomCell.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <numeric>


Board::Board(int size) : boardSize(size) {
    validate_parameters();
}

void Board::validate_parameters() const {
    if (boardSize <= 0) {
        throw std::invalid_argument("Board size must be positive. \n");
    }
}

bool Board::is_valid_cell(int r, int c) const {
 return(r >= 0 && r < boardSize && c >= 0 && c < boardSize);
}

void Board::mark_neighbors(std::vector<unsigned char>& blocked, int row, int col) const {
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            long neighborRow = row + dr;
            long neighborCol = col + dc;

            if (is_valid_cell(neighborRow, neighborCol)) {
                blocked[static_cast<size_t>(neighborRow * boardSize + neighborCol)] = 1;
            }
        }
    }
}

std::size_t Board::free_zone_size(const std::vector<std::pair<int, int>>& selected) const {
    std::vector<unsigned char> blocked(boardSize * boardSize, 0);

    for (const auto& [row, col] : selected) {
        if (row < 0, row >= boardSize, col < 0 || col >= boardSize) {
            throw std::out_of_range("Selected cell is out of board bounds");
        }
        mark_neighbors(blocked, row, col);
    }
    return static_cast<std::size_t>(std::count(blocked.begin(), blocked.end(), 0));
}

double Board::mean(const std::vector<std::size_t>& v) const {
    if (v.empty()) {
        throw std::invalid_argument("vector can't be empty to calculate mean. \n");
    }
    const double sum = static_cast<double>(std::accumulate(v.begin(), v.end(), std::size_t{0}));
    return sum / static_cast<double>(v.size());
}

double Board::median(std::vector<std::size_t> v) const {
    if (v.empty()) {
        throw std::invalid_argument("vector can't be empty to calculate median. \n");
    }
    const std::size_t count = v.size();
    auto mid = v.begin() + count / 2;
    std::nth_element(v.begin(), mid, v.end());
    if (count % 2 == 1) {
        return static_cast<double>(*mid);
    }
    else {
        auto mid2 = std::max_element(v.begin(), mid);
        return (static_cast<double>(*mid) + static_cast<double>(*mid2)) / 2.0;
    }
}
