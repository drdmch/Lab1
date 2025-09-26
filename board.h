#pragma once

#include "randomCell.h"
#include <vector>
//#include <utility>

class Board {
public:
	explicit Board(int size, int cell);

	std::size_t free_zone_size(const std::vector<std::pair<int, int>>& selected) const; //int adj = boardSize

	double mean(const std::vector <std::size_t>& v) const;
	double median(std::vector<std::size_t> v) const;

	void experiment(int exp, double& average, double& median);

private:
	//int boardSize;
	int boardSize;
	int count_selected_cell;

	void validate_parameters() const;
	bool is_neighbor_cell(int r, int c, const std::pair<int, int>& selected) const;
	bool is_valid_cell(int r, int c) const;

	void mark_blocked_all_sides(std::vector<unsigned char>& blocked, int r, int c) const;
	void mark_blocked_vertical_horizontal(std::vector<unsigned char>& blocked, int r, int c) const;


};
