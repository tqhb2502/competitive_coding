#include <iostream>
#include <vector>

using namespace std;

// BIT (Fenwick tree) 1-indexed: hỗ trợ point update và range sum trong O(log n).
class FenwickTree {
public:
    explicit FenwickTree(int n) : bit_(n + 1) {}

    void add(int index, long long delta) {
        for (; index < static_cast<int>(bit_.size()); index += index & -index) {
            bit_[index] += delta;
        }
    }

    long long prefix_sum(int index) const {
        long long result = 0;
        for (; index > 0; index -= index & -index) {
            result += bit_[index];
        }
        return result;
    }

    long long range_sum(int left, int right) const {
        return prefix_sum(right) - prefix_sum(left - 1);
    }

private:
    vector<long long> bit_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, query_count;
    cin >> n >> query_count;
    vector<long long> value(n + 1);
    for (int node = 1; node <= n; ++node) {
        cin >> value[node];
    }

    // Danh sách kề của cây.
    vector<vector<int>> graph(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    // Euler tour bằng DFS lặp từ gốc 1: gán entry (tin) khi vào node, exit (tout)
    // khi đã duyệt hết con. Subtree của node u ứng với đoạn [entry[u], exit[u]].
    vector<int> parent(n + 1, -1), child_index(n + 1);
    vector<int> entry(n + 1), exit(n + 1);
    vector<int> stack = {1};
    parent[1] = 0;
    int timer = 1;
    entry[1] = timer;
    while (!stack.empty()) {
        int node = stack.back();
        if (child_index[node] < static_cast<int>(graph[node].size())) {
            int next = graph[node][child_index[node]++];
            if (next == parent[node]) {
                continue;  // bỏ qua cạnh quay ngược về cha
            }
            parent[next] = node;
            entry[next] = ++timer;  // gán tin cho con khi vừa đi vào
            stack.push_back(next);
        } else {
            exit[node] = timer;  // gán tout khi mọi con đã được thăm
            stack.pop_back();
        }
    }

    // Khởi tạo BIT: đặt giá trị của mỗi node tại vị trí entry[node].
    FenwickTree fenwick(n);
    for (int node = 1; node <= n; ++node) {
        fenwick.add(entry[node], value[node]);
    }

    while (query_count--) {
        int type, node;
        cin >> type >> node;
        if (type == 1) {
            // Đổi giá trị node: cập nhật điểm với delta = giá trị mới - giá trị cũ.
            long long new_value;
            cin >> new_value;
            fenwick.add(entry[node], new_value - value[node]);
            value[node] = new_value;
        } else {
            // Tổng subtree = tổng đoạn [entry[node], exit[node]].
            cout << fenwick.range_sum(entry[node], exit[node]) << '\n';
        }
    }
}
