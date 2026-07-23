#include <iostream>
#include <vector>

using namespace std;

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

    // Danh sách kề của cây
    vector<vector<int>> graph(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    // Euler tour bằng DFS lặp: gán entry[v] (lúc vào) và exit[v] (lúc ra) cho mỗi đỉnh.
    // child_index[node] ghi nhớ đã duyệt tới con thứ mấy của node.
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
                continue;  // Bỏ qua cạnh quay về cha
            }
            parent[next] = node;
            entry[next] = ++timer;  // Vào đỉnh con -> tăng timer
            stack.push_back(next);
        } else {
            exit[node] = timer;  // Ra khỏi node -> chốt exit
            stack.pop_back();
        }
    }

    // Mỗi đỉnh đóng góp trên đoạn Euler mà cây con của nó chiếm giữ.
    // Range-update được biểu diễn bằng hai point update trên BIT hiệu (difference):
    // +value tại entry[v], -value tại exit[v]+1.
    FenwickTree fenwick(n + 1);
    for (int node = 1; node <= n; ++node) {
        fenwick.add(entry[node], value[node]);
        fenwick.add(exit[node] + 1, -value[node]);
    }

    while (query_count--) {
        int type, node;
        cin >> type >> node;
        if (type == 1) {
            // Cập nhật giá trị đỉnh: chỉ cần cộng delta vào hai đầu đoạn của đỉnh
            long long new_value;
            cin >> new_value;
            long long delta = new_value - value[node];
            value[node] = new_value;
            fenwick.add(entry[node], delta);
            fenwick.add(exit[node] + 1, -delta);
        } else {
            // Tổng đường đi gốc -> node = tổng tiền tố tới entry[node]
            cout << fenwick.prefix_sum(entry[node]) << '\n';
        }
    }
}
