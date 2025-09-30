#pragma once

#include <random>
#include <utility>

class RandomCell {
private:
    int boardSize = 0;
    mutable std::mt19937 generator;
    mutable std::uniform_int_distribution<int> distribution; 
public:
    explicit RandomCell(int n);
    std::pair<int, int> operator()() const;
    int getSize() const;
};
