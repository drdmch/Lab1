#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include "board.h"
#include "randomCell.h"

struct ExperimentParameters {
    int boardSize{};
    int numExperiments{};
    int k_min{};
    int k_max{};
};

int get_int_input(const std::string& prompt, int min_value = 0, int max_value = INT_MAX) {
    int value;
    std::cout << prompt;
    std::cin >> value;

    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Invalid input. Please enter a valid number.");
    }

    if (value < min_value || value > max_value) {
        throw std::invalid_argument("Input must be between " + std::to_string(min_value) + " and " + std::to_string(max_value) + ".");
    }

    return value;
}

ExperimentParameters get_user_input() {
    ExperimentParameters p;

    p.boardSize = get_int_input("Enter board size: ", 1);

    p.numExperiments = get_int_input("Enter number of experiments: ", 1);

    while (true) {
        std::cout << "Enter k_min and k_max: ";
        std::cin >> p.k_min >> p.k_max;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Invalid input for k_min/k_max. Please enter valid numbers.\n";
            continue;
        }

        if (p.k_min < 0 ||  p.k_max < 0) {
            std::cerr << "k_min and k_max must be >= 0.\n";
            continue;
        }
        if (p.k_min > p.k_max) {
            std::cerr << "k_min should be <= k_max.\n";
            continue;
        }
        if (p.k_max > p.boardSize * p.boardSize) {
            std::cerr << "k_max cannot be greater than the total number of cells.\n";
            continue;
        }

        break;
    }

    return p;
}

void run_experiment(const ExperimentParameters& p) {
    std::ofstream resultFile("results.csv");
    if (!resultFile.is_open()) throw std::runtime_error("Cannot open results.csv for writing.");

    resultFile << "k,mean_free_cells,median_free_cells,mean_free_pct,median_free_pct\n";

    Board gameBoard(p.boardSize);
    const long long total_cells = static_cast<double>(p.boardSize) * p.boardSize;

    for (int k = p.k_min; k <= p.k_max; ++k) {
        RandomCell cellGenerator(p.boardSize);
        std::vector<std::size_t> freeZoneSizes;
        freeZoneSizes.reserve(static_cast<std::size_t>(p.numExperiments));

        for (int experiment = 0; experiment < p.numExperiments; ++experiment) {
            std::vector<std::pair<int, int>> selectedCells;
            selectedCells.reserve(static_cast<std::size_t>(k));
            for (int i = 0; i < k; ++i) {
                selectedCells.push_back(cellGenerator());
            }
            const std::size_t freeZone = gameBoard.free_zone_size(selectedCells);
            freeZoneSizes.push_back(freeZone);
        }

        const double meanValue = gameBoard.mean(freeZoneSizes);
        const double medianValue = gameBoard.median(freeZoneSizes);
        const double meanPercent = (meanValue / total_cells) * 100.0;
        const double medianPercent = (medianValue / total_cells) * 100.0;

        std::cout << "k = " << k
            << " -> mean: " << meanValue
            << " (" << meanPercent << "%)"
            << ", median: " << medianValue
            << " (" << medianPercent << "%)\n";

        resultFile << k << ',' << meanValue << ',' << medianValue << ','
            << meanPercent << ',' << medianPercent << '\n';
    }

int main() {
    try {
        const ExperimentParameters p = get_user_input();
        run_experiment(p);
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown exception.\n";
        return 1;
    }
}
