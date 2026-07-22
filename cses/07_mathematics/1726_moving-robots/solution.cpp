#include <array>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int steps;
    cin >> steps;

    constexpr size_t SIDE = 8;
    constexpr size_t SQUARES = SIDE * SIDE;
    vector<vector<size_t>> neighbors(SQUARES);
    constexpr array<int, 4> row_delta{1, -1, 0, 0};
    constexpr array<int, 4> column_delta{0, 0, 1, -1};

    for (size_t square = 0; square < SQUARES; ++square) {
        const int row = static_cast<int>(square / SIDE);
        const int column = static_cast<int>(square % SIDE);
        for (size_t direction = 0; direction < row_delta.size(); ++direction) {
            const int next_row = row + row_delta[direction];
            const int next_column = column + column_delta[direction];
            if (0 <= next_row && next_row < static_cast<int>(SIDE) &&
                0 <= next_column && next_column < static_cast<int>(SIDE)) {
                neighbors[square].push_back(
                    static_cast<size_t>(next_row) * SIDE + static_cast<size_t>(next_column));
            }
        }
    }

    vector<vector<double>> distribution(SQUARES, vector<double>(SQUARES));
    for (size_t start = 0; start < SQUARES; ++start) {
        distribution[start][start] = 1.0;
    }

    for (int step = 0; step < steps; ++step) {
        vector<vector<double>> next(SQUARES, vector<double>(SQUARES));
        for (size_t start = 0; start < SQUARES; ++start) {
            for (size_t square = 0; square < SQUARES; ++square) {
                const double share = distribution[start][square] /
                                     static_cast<double>(neighbors[square].size());
                for (size_t neighbor : neighbors[square]) {
                    next[start][neighbor] += share;
                }
            }
        }
        distribution = std::move(next);
    }

    double answer = 0.0;
    for (size_t square = 0; square < SQUARES; ++square) {
        double empty_probability = 1.0;
        for (size_t start = 0; start < SQUARES; ++start) {
            empty_probability *= 1.0 - distribution[start][square];
        }
        answer += empty_probability;
    }

    cout << fixed << setprecision(6) << answer << '\n';
    return 0;
}
