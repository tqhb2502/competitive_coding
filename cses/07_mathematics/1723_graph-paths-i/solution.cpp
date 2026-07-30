#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr long long MOD = 1'000'000'007LL;
using Matrix = vector<vector<long long>>;

// Nhân hai ma trận n x n theo modulo MOD (dùng cho phép bình phương base).
// Bỏ qua các phần tử bằng 0 để tăng tốc.
Matrix multiply(const Matrix& left, const Matrix& right) {
    const size_t n = left.size();
    Matrix product(n, vector<long long>(n));

    for (size_t i = 0; i < n; ++i) {
        for (size_t middle = 0; middle < n; ++middle) {
            if (left[i][middle] == 0) {
                continue;
            }
            const long long coefficient = left[i][middle];
            for (size_t j = 0; j < n; ++j) {
                product[i][j] = (product[i][j] + coefficient * right[middle][j]) % MOD;
            }
        }
    }
    return product;
}

// Nhân vector hàng với ma trận (row * matrix) theo modulo MOD.
// Chỉ cần cập nhật hàng trạng thái thay vì cả ma trận A^k, nhanh hơn.
vector<long long> multiply(const vector<long long>& row, const Matrix& matrix) {
    const size_t n = row.size();
    vector<long long> product(n);
    for (size_t i = 0; i < n; ++i) {
        if (row[i] == 0) {
            continue;
        }
        for (size_t j = 0; j < n; ++j) {
            product[j] = (product[j] + row[i] * matrix[i][j]) % MOD;
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

    // Xây ma trận kề: base[i][j] = số cạnh có hướng đi từ i sang j.
    Matrix base(n, vector<long long>(n));
    for (size_t edge = 0; edge < m; ++edge) {
        size_t from, to;
        cin >> from >> to;
        --from;
        --to;
        base[from][to] = (base[from][to] + 1) % MOD;
    }

    // paths là vector hàng, khởi tạo tại đỉnh 1 (index 0).
    // Nhân dần với A^k bằng binary exponentiation: sau vòng lặp paths = e_0 * A^k,
    // nên paths[n-1] chính là (A^k)[0][n-1] = số walk độ dài k từ đỉnh 1 tới đỉnh n.
    vector<long long> paths(n);
    paths[0] = 1;
    while (k > 0) {
        if (k & 1LL) {
            paths = multiply(paths, base);
        }
        k >>= 1LL;
        if (k > 0) {
            base = multiply(base, base);
        }
    }

    cout << paths[n - 1] << '\n';
    return 0;
}
