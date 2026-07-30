#include <algorithm>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

// Cấu trúc DSU (Union-Find) với path compression và union by size.
class DisjointSet {
public:
    explicit DisjointSet(int n) : parent_(n + 1), size_(n + 1, 1) {
        // Ban đầu mỗi đỉnh là cha của chính nó (mỗi tập chỉ có 1 phần tử).
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    // Tìm đại diện (root) của tập chứa node, kèm nén đường đi.
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

    // Gộp hai tập; trả về kích thước tập sau khi gộp (union by size).
    int unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return size_[a];
        }
        // Gắn cây nhỏ vào cây lớn để giữ cây thấp.
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
    int components = n;         // Số thành phần liên thông hiện tại.
    int largest_component = 1;  // Kích thước thành phần lớn nhất.
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        // Chỉ gộp khi hai thành phố thuộc hai thành phần khác nhau.
        if (dsu.find(a) != dsu.find(b)) {
            largest_component = std::max(largest_component, dsu.unite(a, b));
            --components;
        }
        // In kết quả sau mỗi con đường được xây.
        std::cout << components << ' ' << largest_component << '\n';
    }
    return 0;
}
