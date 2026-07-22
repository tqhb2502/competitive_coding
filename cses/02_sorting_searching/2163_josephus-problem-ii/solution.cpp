#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    long long skipped;
    std::cin >> n >> skipped;

    int size = 1;
    while (size < n) {
        size *= 2;
    }

    std::vector<int> tree(2 * size, 0);
    for (int i = 0; i < n; ++i) {
        tree[size + i] = 1;
    }
    for (int node = size - 1; node >= 1; --node) {
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    long long position = 0;
    for (int remaining = n; remaining >= 1; --remaining) {
        const long long removed_index = (position + skipped) % remaining;
        int target = static_cast<int>(removed_index) + 1;

        int node = 1;
        --tree[node];
        while (node < size) {
            const int left = 2 * node;
            if (tree[left] >= target) {
                node = left;
            } else {
                target -= tree[left];
                node = left + 1;
            }
            --tree[node];
        }

        if (remaining < n) {
            std::cout << ' ';
        }
        std::cout << node - size + 1;
        position = removed_index;
    }
    std::cout << '\n';

    return 0;
}
