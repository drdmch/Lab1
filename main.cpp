#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "board.h"
#include "randomCell.h"

struct ExperimentParameters {
    int boardSize{};
    int numExperiments{};
    int k_min{};
    int k_max{};
};

ExperimentParameters get_user_input() {
    ExperimentParameters p;

    std::cout << "Enter board size: " << std::endl;
    std::cin >> p.boardSize;
    if (!(std::cin >> p.boardSize)) throw std::invalid_argument("Invalid input for board size.");
    if (p.boardSize <= 0) throw std::invalid_argument("Board size must be positive.");

    std::cout << "Enter number of experiments: ";
    std::cin >> p.numExperiments;
    if (!(std::cin >> p.numExperiments)) throw std::invalid_argument("Invalid input for number of experiments.");
    if (p.numExperiments <= 0) throw std::invalid_argument("Number of experiments must be positive.");

    std::cout << "Enter k_min and k_max: ";
    std::cin >> p.k_min >> p.k_max;
    if (!(std::cin >> p.k_min >> p.k_max)) throw std::invalid_argument("Invalid input for k_min/k_max.");
    if (p.k_min < 0 || p.k_max < 0) throw std::invalid_argument("k_min and k_max must be >= 0.");
    if (p.k_min > p.k_max) throw std::invalid_argument("k_min should be <= k_max.");
    if (p.k_max > p.boardSize*p.boardSize) throw std::invalid_argument("k_max cannot be > total number od cells.");

    return p;
}

void run_experiment(const ExperimentParameters& p) {
    std::ofstream resultFile("results.csv");
    if (!resultFile.is_open()) throw std::runtime_error("Cannot open results.csv for writing.");

    resultFile << "k,mean_free_cells,median_free_cells,mean_free_pct,median_free_pct\n";

    Board gameBoard(p.boardSize);
    const double total_cells = static_cast<double>(p.boardSize) * p.boardSize;

    for (int k = p.k_min; k <= p.k_max; ++k) {
        RandomCell cellGenerator(p.boardSize);
        std::vector<std::size_t> freeZoneSizes;
        freeZoneSizes.reserve(static_cast<std::size_t>(p.numExperiments));

        for (int experiment = 0; experiment < p.numExperiments; ++experiment) {
            std::vector<std::pair<int,int>> selectedCells;
            selectedCells.reserve(static_cast<std::size_t>(k));
            for (int i = 0; i < k; ++i) {
                selectedCells.push_back(cellGenerator());
            }
            const std::size_t freeZone = gameBoard.free_zone_size(selectedCells);
            freeZoneSizes.push_back(freeZone);
        }

        const double meanValue   = gameBoard.mean(freeZoneSizes);
        const double medianValue = gameBoard.median(freeZoneSizes);
        const double meanPercent     = (meanValue   / total_cells) * 100.0;
        const double medianPercent   = (medianValue / total_cells) * 100.0;

        std::cout << "k = " << k
                  << " -> mean: " << meanValue
                  << " (" << meanPercent   << "%)"
                  << ", median: " << medianValue
                  << " (" << medianPercent << "%)\n";

        resultFile << k << ',' << meanValue << ',' << medianValue << ','
                   << meanPercent << ',' << medianPercent << '\n';
    }

    std::cout << "\nExperiment completed. Results saved to 'results.csv'." << "\n";
}

int main() {
    try {
        const ExperimentParameters p = get_user_input();
        run_experiment(p);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception.\n";
        return 1;
    }
}
