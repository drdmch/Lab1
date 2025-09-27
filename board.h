#pragma once

#include <vector>
#include <utility>

class Board{
private:
    int boardSize;

    void mark_neighbors(std::vector<unsigned char>& blocked, int r, int c) const;
    void validate_parameters() const;
    bool is_valid_cell(int r, int c) const;
public:
    explicit Board(int n);

    std::size_t free_zone_size(const std::vector<std::pair<int, int>>& selected) const;

    double mean(const std::vector < std::size_t>& v) const;
    double median(std::vector<std::size_t> v) const;

    int size() const {
        return boardSize;
    };
};
