#include "board.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
//#include <numeric>
#include <ostream>

Board::Board(int size, int cell) : boardSize(size), count_selected_cell(cell) {
    validate_parameters();
}

void Board::validate_parameters() const {
    if (boardSize <= 0) {
        throw std::invalid_argument("Board size must be positive. \n");
    }
    if(boardSize > 1000) {
        throw std::invalid_argument("Too large board may slow down calculation. \n ");
    }
    if (count_selected_cell > boardSize * boardSize) {
        throw std::invalid_argument("Number of selected cells must be smaller than board size. \n");
    }
}

bool Board::is_valid_cell(int r, int c) const {
 return(r >= 0 && r < boardSize && c >= 0 && c < boardSize);
}

bool Board::is_neighbor_cell(int r, int c, const std::pair<int, int>& selected) const {
    if (!is_valid_cell(r, c) || is_valid_cell(selected.first, selected.second)) {
        return false;
    }
    int x = std::abs(r -selected.first );
    int y = std::abs(c - selected.second );
    return (x <= 1 && y <= 1 && (x != 0 || y != 0)); // ==2
}


void Board::mark_blocked_all_sides(std::vector<unsigned char>& blocked, int row, int col) const {
    for (int dr = -1; dr <= 1; ++dr) {
        long neighborRow = static_cast<long>(row) + dr;
        if (neighborRow < 0, neighborRow >= boardSize) continue;

        for (int dc = -1; dc <= 1; ++dc) {
            long neighborCol = static_cast<long>(col) + dc;
            if (neighborCol < 0, neighborCol >= boardSize) continue;

            blocked[static_cast<size_t>(neighborRow * boardSize + neighborCol)] = 1;
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
    mark(row - 1, col);
    mark(row + 1, col);
    mark(row, col - 1);
    mark(row, col + 1);
}

std::size_t Board::free_zone_size(const std::vector<std::pair<int, int>>& selected, int adj) const {
    std::vector<unsigned char> blocked(boardSize * boardSize, 0);

    for (const auto& selectedCell : selected) {
        const int row = selectedCell.first;
        const int col = selectedCell.second;
        if (row < 0, row >= boardSize, col < 0 || col >= boardSize) {
            throw std::out_of_range("Selected cell is out of board bounds");
        }

        if (adj == boardSize) {
            mark_blocked_all_sides(blocked, row, col);
        }
        else {
            mark_blocked_vertical_horizontal(blocked, row, col);
        }
    }

    return static_cast<std::size_t>(std::count(blocked.begin(), blocked.end(), 0));
}

double Board::mean(const std::vector<std::size_t>& v) const {
    if (v.empty()) throw std::invalid_argument("vector can't be empty to calculate average. \n");
    double sum = 0;
    for (size_t x : v) {
        sum += static_cast<long double>(x); //std::accumulate(v.begin(), v.end(), 0.0);std::accumulate(v.begin(), v.end(), 0.0);
    }
    return sum / v.size();
}

double Board::median(std::vector<std::size_t> v) const {
    if (v.empty()) throw std::invalid_argument("vector can't be empty to calculate median. \n");
    const size_t count = v.size();
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

void Board::experiment(int exp, double &average, double &median) {
    if (exp <= 0) {
        throw std::invalid_argument("experiment must be positive. \n");
    }
    RandomCell rand_cell(boardSize);
    std::vector<std::size_t> result;
    result.reserve(exp);
    int count = 0;

    for(int i = 0; i < exp; ++i) {
        try {
            std::vector<std::pair<int, int>> selected;
            for(int j = 0; j < count_selected_cell; ++j) {
                selected.push_back(rand_cell());
            }

            std::size_t zone_size = free_zone_size(selected, boardSize);
            result.push_back(zone_size());
            count ++;
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    if(count == 0) {
        throw std::invalid_argument("No successful  experiment found. \n");
    }
    average = mean(result);
    median = median(result);

    std::cout << "Successful experiments: " << count << std::endl;

}
