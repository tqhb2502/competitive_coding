#include <algorithm>
#include <iostream>
#include <numeric>
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

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return false;
        }
        if (size_[a] < size_[b]) {
            std::swap(a, b);
        }
        parent_[b] = a;
        size_[a] += size_[b];
        return true;
    }

private:
    std::vector<int> parent_;
    std::vector<int> size_;
};

struct Road {
    int from;
    int to;
    long long cost;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<Road> roads;
    roads.reserve(static_cast<std::size_t>(m));
    for (int i = 0; i < m; ++i) {
        Road road{};
        std::cin >> road.from >> road.to >> road.cost;
        roads.push_back(road);
    }

    std::sort(roads.begin(), roads.end(), [](const Road& lhs, const Road& rhs) {
        return lhs.cost < rhs.cost;
    });

    DisjointSet dsu(n);
    long long total_cost = 0;
    int used_edges = 0;
    for (const Road& road : roads) {
        if (dsu.unite(road.from, road.to)) {
            total_cost += road.cost;
            ++used_edges;
            if (used_edges == n - 1) {
                break;
            }
        }
    }

    if (used_edges != n - 1) {
        std::cout << "IMPOSSIBLE\n";
    } else {
        std::cout << total_cost << '\n';
    }
    return 0;
}
