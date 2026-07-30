#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

namespace {
// INF lớn hơn đáp án thật tối đa (k * max_c <= 1e18) để đánh dấu "không có đường".
constexpr long long INF = 1LL << 62;
using Matrix = vector<vector<long long>>;

// Tích ma trận trên tropical semiring (min, +):
//   product[i][j] = min_middle ( left[i][middle] + right[middle][j] ).
// Bỏ qua node trung gian có giá trị INF để tránh cộng tràn và giữ ô INF nguyên vẹn.
Matrix multiply(const Matrix& left, const Matrix& right) {
    const size_t n = left.size();
    Matrix product(n, vector<long long>(n, INF));

    for (size_t i = 0; i < n; ++i) {
        for (size_t middle = 0; middle < n; ++middle) {
            if (left[i][middle] == INF) {
                continue;
            }
            for (size_t j = 0; j < n; ++j) {
                if (right[middle][j] == INF) {
                    continue;
                }
                product[i][j] = min(product[i][j], left[i][middle] + right[middle][j]);
            }
        }
    }
    return product;
}

// Nhân vector hàng với ma trận trong min-plus, O(n^2):
//   product[j] = min_i ( row[i] + matrix[i][j] ).
vector<long long> multiply(const vector<long long>& row, const Matrix& matrix) {
    const size_t n = row.size();
    vector<long long> product(n, INF);
    for (size_t i = 0; i < n; ++i) {
        if (row[i] == INF) {
            continue;
        }
        for (size_t j = 0; j < n; ++j) {
            if (matrix[i][j] != INF) {
                product[j] = min(product[j], row[i] + matrix[i][j]);
            }
        }
    }
    return product;
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    size_t n, m;
    long long k;
    cin >> n >> m >> k;

    // base[i][j] = trọng số cạnh nhỏ nhất từ i -> j (INF nếu không có cạnh).
    Matrix base(n, vector<long long>(n, INF));
    for (size_t edge = 0; edge < m; ++edge) {
        size_t from, to;
        long long weight;
        cin >> from >> to >> weight;
        --from;
        --to;
        base[from][to] = min(base[from][to], weight);
    }

    // distance = vector hàng e_0 (bắt đầu tại node 0). Fast exponentiation:
    // với mỗi bit 1 của k thì nhân vector vào lũy thừa hiện tại (O(n^2)),
    // còn base bình phương lên O(log k) lần (O(n^3)). Sau vòng lặp:
    // distance[j] = trọng số nhỏ nhất của walk 0->j dùng đúng k cạnh.
    vector<long long> distance(n, INF);
    distance[0] = 0;
    while (k > 0) {
        if (k & 1LL) {
            distance = multiply(distance, base);
        }
        k >>= 1LL;
        if (k > 0) {
            base = multiply(base, base);
        }
    }

    // distance vẫn bằng INF => không có walk 0->(n-1) đúng k cạnh.
    if (distance[n - 1] == INF) {
        cout << -1 << '\n';
    } else {
        cout << distance[n - 1] << '\n';
    }
    return 0;
}
