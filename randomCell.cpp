#include "randomCell.h"
#include <stdexcept>

RandomCell::RandomCell(int n) : boardSize(n), distribution(0, n - 1) {
    if (boardSize <= 0) {
        throw std::invalid_argument("Board size must be positive.");
    }
    if (boardSize > 1000) {
        throw std::invalid_argument("Board size exceeds the maximum allowed size of 1000."); //під питанням!!!! ВИПРАВИЛА
    }
    std::random_device rand;
    generator = std::mt19937(rand());
    }

std::pair<int, int> RandomCell::operator()() const {
   
    if (board_size == 1) {
        return { 0, 0 };
    }
    int row = distribution(generator);
    int col = distribution(generator);
    return {row, col};
}

int RandomCell::getSize() const {
    return boardSize;
}



