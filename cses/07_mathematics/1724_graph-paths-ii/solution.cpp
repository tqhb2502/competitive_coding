#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr long long INF = 1LL << 62;
using Matrix = vector<vector<long long>>;

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

    Matrix base(n, vector<long long>(n, INF));
    for (size_t edge = 0; edge < m; ++edge) {
        size_t from, to;
        long long weight;
        cin >> from >> to >> weight;
        --from;
        --to;
        base[from][to] = min(base[from][to], weight);
    }

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

    if (distance[n - 1] == INF) {
        cout << -1 << '\n';
    } else {
        cout << distance[n - 1] << '\n';
    }
    return 0;
}
