#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

class DisjointSetUnion {
public:
    explicit DisjointSetUnion(int n) : parent_(n + 1), size_(n + 1, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int vertex) {
        int root = vertex;
        while (parent_[root] != root) {
            root = parent_[root];
        }
        while (parent_[vertex] != vertex) {
            const int next = parent_[vertex];
            parent_[vertex] = root;
            vertex = next;
        }
        return root;
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return;
        }
        if (size_[a] < size_[b]) {
            std::swap(a, b);
        }
        parent_[b] = a;
        size_[a] += size_[b];
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

    DisjointSetUnion dsu(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        dsu.unite(a, b);
    }

    std::vector<int> representatives;
    for (int vertex = 1; vertex <= n; ++vertex) {
        if (dsu.find(vertex) == vertex) {
            representatives.push_back(vertex);
        }
    }

    std::cout << representatives.size() - 1 << '\n';
    for (int i = 1; i < static_cast<int>(representatives.size()); ++i) {
        std::cout << representatives[0] << ' ' << representatives[i] << '\n';
    }

    return 0;
}
