#include "randomCell.h"
#include <stdexcept>

RandomCell::RandomCell(int n) : boardSize(n), distribution(0, n - 1) {
    if (boardSize <= 0) {
        throw std::invalid_argument("Board size must be positive.");
    }
    if (boardSize > 1000) {
        throw std::invalid_argument("Random cell size doesn't initialise correctly"); //під питанням!!!!
    }
    std::random_device rand;
    generator = std::mt19937(rand());

//     unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
//     generator.seed(seed);
    }

std::pair<int, int> RandomCell::operator()() const {
    int cellIndex = distribution(generator);
    int row = cellIndex / boardSize;
    int col = cellIndex % boardSize;
    return {row, col};
}

int RandomCell::getSize() const {
    return boardSize;
}



