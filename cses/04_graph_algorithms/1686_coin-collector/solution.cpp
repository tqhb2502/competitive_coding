#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    // Số xu tại mỗi phòng.
    std::vector<long long> coins(n);
    for (long long& value : coins) {
        std::cin >> value;
    }

    // Danh sách kề adj cho Tarjan, đồng thời lưu lại toàn bộ cạnh gốc để condense.
    std::vector<std::vector<int>> adj(n);
    std::vector<std::pair<int, int>> edges;
    edges.reserve(m);
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        --a;
        --b;
        adj[a].push_back(b);
        edges.emplace_back(a, b);
    }

    // Bước 1: Tìm SCC bằng Tarjan iterative (stack tường minh, không đệ quy).
    // Một SCC được đánh số khi nó bị pop, nên mọi cạnh giữa các SCC luôn đi từ
    // id lớn sang id nhỏ (thứ tự topo đảo ngược).
    std::vector<int> index(n, -1);
    std::vector<int> low(n, 0);
    std::vector<int> component(n, -1);
    std::vector<std::size_t> next_edge(n, 0);  // con trỏ cạnh đang xét của mỗi đỉnh
    std::vector<char> on_stack(n, false);
    std::vector<int> scc_stack;  // stack các đỉnh của SCC đang mở
    std::vector<int> dfs_stack;  // stack mô phỏng đệ quy DFS
    scc_stack.reserve(n);
    dfs_stack.reserve(n);

    int timer = 0;
    int component_count = 0;
    for (int start = 0; start < n; ++start) {
        if (index[start] != -1) {
            continue;
        }

        index[start] = low[start] = timer++;
        on_stack[start] = true;
        scc_stack.push_back(start);
        dfs_stack.push_back(start);

        while (!dfs_stack.empty()) {
            const int node = dfs_stack.back();
            if (next_edge[node] < adj[node].size()) {
                const int next = adj[node][next_edge[node]++];
                if (index[next] == -1) {
                    // Cạnh cây: đi sâu xuống đỉnh chưa thăm.
                    index[next] = low[next] = timer++;
                    on_stack[next] = true;
                    scc_stack.push_back(next);
                    dfs_stack.push_back(next);
                } else if (on_stack[next]) {
                    // Cạnh ngược tới đỉnh còn trên stack: cập nhật low.
                    low[node] = std::min(low[node], index[next]);
                }
                continue;
            }

            // Đã duyệt hết cạnh của node: quay lui.
            dfs_stack.pop_back();
            if (low[node] == index[node]) {
                // node là gốc của một SCC: pop toàn bộ thành phần.
                while (true) {
                    const int member = scc_stack.back();
                    scc_stack.pop_back();
                    on_stack[member] = false;
                    component[member] = component_count;
                    if (member == node) {
                        break;
                    }
                }
                ++component_count;
            }
            // Truyền low của con lên cha.
            if (!dfs_stack.empty()) {
                const int parent = dfs_stack.back();
                low[parent] = std::min(low[parent], low[node]);
            }
        }
    }

    // Bước 2: Giá trị mỗi SCC = tổng xu các phòng thuộc SCC đó.
    std::vector<long long> component_coins(component_count, 0);
    for (int room = 0; room < n; ++room) {
        component_coins[component[room]] += coins[room];
    }

    // Bước 3: Danh sách kề của DAG condense (chỉ giữ cạnh nối hai SCC khác nhau).
    std::vector<std::vector<int>> condensed(component_count);
    for (const auto& [from, to] : edges) {
        const int from_component = component[from];
        const int to_component = component[to];
        if (from_component != to_component) {
            condensed[from_component].push_back(to_component);
        }
    }

    // Bước 4: DP đường đi tổng lớn nhất trên DAG. Vì id SCC theo thứ tự topo đảo
    // ngược, mọi successor của c có id nhỏ hơn nên đã được tính trước khi tới c.
    std::vector<long long> best(component_count, 0);
    long long answer = 0;
    for (int c = 0; c < component_count; ++c) {
        long long best_successor = 0;
        for (const int next : condensed[c]) {
            best_successor = std::max(best_successor, best[next]);
        }
        best[c] = component_coins[c] + best_successor;
        answer = std::max(answer, best[c]);
    }

    std::cout << answer << '\n';
    return 0;
}
