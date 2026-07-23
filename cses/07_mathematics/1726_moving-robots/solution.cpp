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

    // Dựng danh sách láng giềng hợp lệ cho mỗi ô; bậc của ô = số hướng đi được
    // (ô góc 2 hướng, ô cạnh 3 hướng, ô trong 4 hướng).
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

    // distribution[start][.] = phân bố xác suất vị trí của robot xuất phát tại ô start;
    // khởi tạo phân bố đặt toàn bộ xác suất tại chính ô xuất phát.
    vector<vector<double>> distribution(SQUARES, vector<double>(SQUARES));
    for (size_t start = 0; start < SQUARES; ++start) {
        distribution[start][start] = 1.0;
    }

    // Mô phỏng random walk k bước: mỗi bước, xác suất tại ô square được chia đều
    // p/deg(square) sang từng ô láng giềng (tương đương nhân với ma trận chuyển T).
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

    // Linearity of expectation: đáp số = tổng trên mọi ô của P(ô đó rỗng); các robot
    // di chuyển độc lập nên P(ô rỗng) = tích (1 - p_k(start -> ô)) trên mọi ô xuất phát.
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
