#pragma once
#include <bits/stdc++.h>
using namespace std;
// Edge Orientation — định hướng vô hướng theo thứ tự để tạo DAG, hoặc để mọi outdegree chẵn.
// Khi dùng: cần hướng cạnh không chu trình; hoặc chọn hướng sao cho bậc ra mỗi đỉnh có parity 0.
// ĐPT: O(n+m); bộ nhớ O(n+m). Đỉnh 0-based; hỗ trợ cạnh song song, không hỗ trợ self-loop ở bản parity.
// Dùng: orientAcyclicByRank(edges, rank); auto r=orientEvenOutdegree(n, edges);
// Bẫy: even-outdegree tồn tại iff mỗi thành phần có số cạnh chẵn; output giữ đúng thứ tự edge input.
// CSES: 1756 2179
struct EdgeOrientationResult {
    bool possible = false;
    vector<pair<int, int>> directed;
};

// Ý nghĩa: hướng mỗi cạnh từ rank nhỏ sang rank lớn, do đó kết quả luôn là DAG nếu rank là hoán vị.
// Tham số: edges = cạnh vô hướng; rank[v] = thứ tự topo phân biệt của v.
// Trả về: cạnh có hướng theo đúng thứ tự input.
vector<pair<int, int>> orientAcyclicByRank(const vector<pair<int, int>>& edges,
                                           const vector<int>& rank) {
    int n = (int)rank.size();
    vector<char> seen(n, false);
    for (int value : rank) {
        if (value < 0 || value >= n || seen[value])
            throw invalid_argument("orientAcyclicByRank: rank không là hoán vị 0..n-1");
        seen[value] = true;
    }
    vector<pair<int, int>> answer;
    answer.reserve(edges.size());
    for (auto [u, v] : edges) {
        if (u < 0 || u >= n || v < 0 || v >= n || u == v)
            throw invalid_argument("orientAcyclicByRank: cạnh không hợp lệ");
        if (rank[u] < rank[v]) answer.push_back({u, v});
        else answer.push_back({v, u});
    }
    return answer;
}

// Ý nghĩa: hướng cạnh để bậc ra của mọi đỉnh chẵn bằng DFS forest và sửa parity từ lá lên gốc.
// Tham số: n = số đỉnh; edges = cạnh vô hướng 0-based, có thể song song nhưng không self-loop.
// Trả về: possible và danh sách cạnh hướng cùng thứ tự input.
EdgeOrientationResult orientEvenOutdegree(int n, const vector<pair<int, int>>& edges) {
    if (n < 0) throw invalid_argument("orientEvenOutdegree: n âm");
    int m = (int)edges.size();
    vector<vector<pair<int, int>>> adj(n);
    for (int id = 0; id < m; ++id) {
        auto [u, v] = edges[id];
        if (u < 0 || u >= n || v < 0 || v >= n || u == v)
            throw invalid_argument("orientEvenOutdegree: cạnh không hợp lệ");
        adj[u].push_back({v, id});
        adj[v].push_back({u, id});
    }
    vector<pair<int, int>> directed(m, {-1, -1});
    vector<char> visited(n, false), usedEdge(m, false), parity(n, false);
    vector<int> parent(n, -1), parentEdge(n, -1), nextEdge(n, 0);
    for (int root = 0; root < n; ++root) if (!visited[root]) {
        visited[root] = true;
        vector<int> stack = {root};
        while (!stack.empty()) {
            int u = stack.back();
            if (nextEdge[u] < (int)adj[u].size()) {
                auto [v, id] = adj[u][nextEdge[u]++];
                if (usedEdge[id]) continue;
                usedEdge[id] = true;
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    parentEdge[v] = id;
                    stack.push_back(v);
                } else {
                    directed[id] = {u, v};
                    parity[u] = !parity[u];
                }
                continue;
            }
            stack.pop_back();
            if (parentEdge[u] != -1) {
                int p = parent[u], id = parentEdge[u];
                if (parity[u]) {
                    directed[id] = {u, p};
                    parity[u] = false;
                } else {
                    directed[id] = {p, u};
                    parity[p] = !parity[p];
                }
            } else if (parity[u]) {
                return {false, {}};
            }
        }
    }
    return {true, std::move(directed)};
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ edge-orientation.hpp -o demo && ./demo
int main() {
    vector<pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 0}, {0, 3}};
    auto result = orientEvenOutdegree(4, edges);
    vector<int> out(4, 0);
    for (auto [u, v] : result.directed) { (void)v; ++out[u]; }
    printf("possible=%d; outdegree: %d %d %d %d\n",
           (int)result.possible, out[0], out[1], out[2], out[3]);
    return 0;
}
#endif
