#pragma once
#include <bits/stdc++.h>
using namespace std;
// DFS Iterative — rừng DFS không đệ quy, thứ tự vào/ra, cha và thành phần liên thông.
// Khi dùng: flood fill/thành phần; duyệt sâu đồ thị tới 2e5+ đỉnh mà recursion có thể tràn stack.
// ĐPT: O(n+m); bộ nhớ O(n+m) gồm adj và stack. Đỉnh 0-based.
// Dùng: auto forest=dfsForest(adj); forest.component[v]; countGridComponents(grid,'.');
// Bẫy: đồ thị có hướng vẫn duyệt reachability theo chiều cạnh; exitOrder là postorder; parent không duy nhất.
// CSES: 1192 1704 2179
struct DFSForestResult {
    vector<int> parent;
    vector<int> component;
    vector<int> enterOrder;
    vector<int> exitOrder;
};

// Ý nghĩa: dựng rừng DFS lặp trên mọi đỉnh, ghi thứ tự vào và kết thúc.
// Tham số: adj = danh sách kề 0-based (có hướng hoặc vô hướng).
// Trả về: parent (-1 tại gốc), id thành phần, preorder và postorder.
DFSForestResult dfsForest(const vector<vector<int>>& adj) {
    int n = (int)adj.size(), components = 0;
    DFSForestResult result;
    result.parent.assign(n, -1);
    result.component.assign(n, -1);
    vector<int> nextEdge(n, 0);
    for (int root = 0; root < n; ++root) if (result.component[root] == -1) {
        result.component[root] = components++;
        result.enterOrder.push_back(root);
        vector<int> stack = {root};
        while (!stack.empty()) {
            int u = stack.back();
            if (nextEdge[u] == (int)adj[u].size()) {
                result.exitOrder.push_back(u);
                stack.pop_back();
                continue;
            }
            int v = adj[u][nextEdge[u]++];
            if (result.component[v] != -1) continue;
            result.component[v] = result.component[u];
            result.parent[v] = u;
            result.enterOrder.push_back(v);
            stack.push_back(v);
        }
    }
    return result;
}

// Ý nghĩa: đếm thành phần 4-hướng của các ô mang ký tự open bằng flood fill lặp.
// Tham số: grid = lưới chữ nhật; open = ký tự được coi là đỉnh.
// Trả về: số thành phần liên thông của các ô open.
int countGridComponents(const vector<string>& grid, char open = '.') {
    if (grid.empty()) return 0;
    int n = (int)grid.size(), m = (int)grid[0].size();
    for (const string& row : grid) if ((int)row.size() != m)
        throw invalid_argument("countGridComponents: lưới không chữ nhật");
    vector<char> seen(n * m, false);
    int components = 0;
    constexpr int dr[4] = {-1, 1, 0, 0};
    constexpr int dc[4] = {0, 0, -1, 1};
    for (int r = 0; r < n; ++r) for (int c = 0; c < m; ++c) {
        int start = r * m + c;
        if (grid[r][c] != open || seen[start]) continue;
        ++components;
        seen[start] = true;
        vector<int> stack = {start};
        while (!stack.empty()) {
            int at = stack.back();
            stack.pop_back();
            int x = at / m, y = at % m;
            for (int d = 0; d < 4; ++d) {
                int nx = x + dr[d], ny = y + dc[d];
                if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
                int to = nx * m + ny;
                if (!seen[to] && grid[nx][ny] == open) {
                    seen[to] = true;
                    stack.push_back(to);
                }
            }
        }
    }
    return components;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ dfs.hpp -o demo && ./demo
int main() {
    vector<vector<int>> adj = {{1}, {0}, {3}, {2}, {}};
    auto forest = dfsForest(adj);
    vector<string> grid = {"..#", "#.#", "..#"};
    printf("graph components=%d; rooms=%d; postorder=%zu\n",
           1 + *max_element(forest.component.begin(), forest.component.end()),
           countGridComponents(grid), forest.exitOrder.size());  // 3, 1, 5
    return 0;
}
#endif
