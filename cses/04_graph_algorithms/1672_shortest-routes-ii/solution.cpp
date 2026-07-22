#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m, q;
    std::cin >> n >> m >> q;

    constexpr long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<std::vector<long long>> distance(
        n, std::vector<long long>(n, INF));
    for (int i = 0; i < n; ++i) {
        distance[i][i] = 0;
    }

    for (int i = 0; i < m; ++i) {
        int a, b;
        long long weight;
        std::cin >> a >> b >> weight;
        --a;
        --b;
        distance[a][b] = std::min(distance[a][b], weight);
        distance[b][a] = std::min(distance[b][a], weight);
    }

    for (int k = 0; k < n; ++k) {
        const std::vector<long long>& via = distance[k];
        for (int i = 0; i < n; ++i) {
            std::vector<long long>& row = distance[i];
            const long long to_k = row[k];
            if (to_k == INF) {
                continue;
            }
            for (int j = 0; j < n; ++j) {
                const long long through_k = to_k + via[j];
                if (through_k < row[j]) {
                    row[j] = through_k;
                }
            }
        }
    }

    for (int query = 0; query < q; ++query) {
        int a, b;
        std::cin >> a >> b;
        const long long answer = distance[a - 1][b - 1];
        std::cout << (answer == INF ? -1 : answer) << '\n';
    }
    return 0;
}
