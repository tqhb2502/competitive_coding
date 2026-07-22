#include <algorithm>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

class DisjointSet {
public:
    explicit DisjointSet(int n) : parent_(n + 1), size_(n + 1, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int node) {
        int root = node;
        while (parent_[root] != root) {
            root = parent_[root];
        }
        while (parent_[node] != node) {
            const int next = parent_[node];
            parent_[node] = root;
            node = next;
        }
        return root;
    }

    int unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return size_[a];
        }
        if (size_[a] < size_[b]) {
            std::swap(a, b);
        }
        parent_[b] = a;
        size_[a] += size_[b];
        return size_[a];
    }

private:
    std::vector<int> parent_;
    std::vector<int> size_;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    DisjointSet dsu(n);
    int components = n;
    int largest_component = 1;
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        if (dsu.find(a) != dsu.find(b)) {
            largest_component = std::max(largest_component, dsu.unite(a, b));
            --components;
        }
        std::cout << components << ' ' << largest_component << '\n';
    }
    return 0;
}
