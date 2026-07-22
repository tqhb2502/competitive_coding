// Hotel Queries - CSES 1143
// Max segment tree: descend to the leftmost hotel with enough rooms.

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::size_t n = 0;
    std::size_t m = 0;
    std::cin >> n >> m;

    std::size_t size = 1;
    while (size < n) {
        size *= 2;
    }

    std::vector<long long> tree(2 * size, 0);
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> tree[size + i];
    }
    for (std::size_t node = size; node-- > 1;) {
        tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
    }

    for (std::size_t query = 0; query < m; ++query) {
        long long rooms = 0;
        std::cin >> rooms;

        std::size_t answer = 0;
        if (tree[1] >= rooms) {
            std::size_t node = 1;
            while (node < size) {
                if (tree[2 * node] >= rooms) {
                    node *= 2;
                } else {
                    node = 2 * node + 1;
                }
            }

            answer = node - size + 1;
            tree[node] -= rooms;
            for (node /= 2; node >= 1; node /= 2) {
                tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
            }
        }

        if (query > 0) {
            std::cout << ' ';
        }
        std::cout << answer;
    }
    std::cout << '\n';
    return 0;
}
