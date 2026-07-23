#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

// Cấu trúc DSU (Union-Find) để quản lý các thành phần liên thông.
class DisjointSetUnion {
public:
    explicit DisjointSetUnion(int n) : parent_(n + 1), size_(n + 1, 1) {
        // Ban đầu mỗi đỉnh là gốc của chính nó.
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    // Tìm gốc đại diện của đỉnh, kèm path compression.
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

    // Gộp hai thành phần, dùng union by size để giữ cây thấp.
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

    // Gộp các đỉnh nối bởi những con đường đã có sẵn.
    DisjointSetUnion dsu(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        dsu.unite(a, b);
    }

    // Lấy một đỉnh đại diện (root) cho mỗi thành phần liên thông.
    std::vector<int> representatives;
    for (int vertex = 1; vertex <= n; ++vertex) {
        if (dsu.find(vertex) == vertex) {
            representatives.push_back(vertex);
        }
    }

    // Cần (số thành phần - 1) con đường mới; nối đại diện đầu tiên với các đại diện còn lại.
    std::cout << representatives.size() - 1 << '\n';
    for (int i = 1; i < static_cast<int>(representatives.size()); ++i) {
        std::cout << representatives[0] << ' ' << representatives[i] << '\n';
    }

    return 0;
}
