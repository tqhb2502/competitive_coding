#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

// Cấu trúc Disjoint Set Union (DSU) dùng để quản lý các thành phần liên thông.
class DisjointSet {
public:
    explicit DisjointSet(int n) : parent_(n + 1), size_(n + 1, 1) {
        // Ban đầu mỗi đỉnh là gốc của chính nó.
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    // Tìm gốc đại diện của thành phần chứa node, kèm path compression.
    int find(int node) {
        int root = node;
        while (parent_[root] != root) {
            root = parent_[root];
        }
        // Nén đường: trỏ thẳng mọi đỉnh trên đường về gốc.
        while (parent_[node] != node) {
            const int next = parent_[node];
            parent_[node] = root;
            node = next;
        }
        return root;
    }

    // Gộp hai thành phần theo union by size; trả về false nếu đã cùng thành phần.
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

// Một con đường: nối from - to với chi phí sửa chữa cost.
struct Road {
    int from;
    int to;
    long long cost;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Đọc số thành phố n và số con đường m, sau đó đọc danh sách cạnh.
    int n, m;
    std::cin >> n >> m;
    std::vector<Road> roads;
    roads.reserve(static_cast<std::size_t>(m));
    for (int i = 0; i < m; ++i) {
        Road road{};
        std::cin >> road.from >> road.to >> road.cost;
        roads.push_back(road);
    }

    // Kruskal: sắp xếp các cạnh theo chi phí tăng dần.
    std::sort(roads.begin(), roads.end(), [](const Road& lhs, const Road& rhs) {
        return lhs.cost < rhs.cost;
    });

    // Duyệt cạnh từ rẻ nhất, gộp hai thành phần khác nhau để dựng MST.
    DisjointSet dsu(n);
    long long total_cost = 0;
    int used_edges = 0;
    for (const Road& road : roads) {
        if (dsu.unite(road.from, road.to)) {
            total_cost += road.cost;
            ++used_edges;
            // Đã đủ n-1 cạnh: cây khung hoàn chỉnh, dừng sớm.
            if (used_edges == n - 1) {
                break;
            }
        }
    }

    // Chọn đủ n-1 cạnh nghĩa là đồ thị liên thông; ngược lại thì không thể nối.
    if (used_edges != n - 1) {
        std::cout << "IMPOSSIBLE\n";
    } else {
        std::cout << total_cost << '\n';
    }
    return 0;
}
