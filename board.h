#pragma once

#include <vector>
#include <utility>

const int ALL_SIDES = 8; //¬ключаЇмо сюди д≥агональн≥
const int ONLY_VERTICAL_HORIZONTAL = 4;

class Board {
public:
	explicit Board(int n);

	std::size_t free_zona_size(const std::vector<std::pair<int, int>>& selected,
		int adj = ALL_SIDES) const;

	double mean(const std::vector < std::size_t& v) const;
	double median(std::vector<std::size_t>) const;

private:
	int boardSize;

	void mark_blocked_all_sides(std::vector<unsigned char>& blocked, int r, int c) const;
	void mark_blocked_vertical_horizontal(std::vector<unsigned char>& blocked, int r, int c) const;
};
