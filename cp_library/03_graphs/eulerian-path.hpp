#pragma once
#include <bits/stdc++.h>
using namespace std;
// Đường đi Euler (Hierholzer) — đi qua MỌI cạnh đúng một lần; bản có hướng và vô hướng, O(n + m).
// Khi dùng: cần lộ trình dùng hết cạnh (đường/chu trình Euler).
// ĐPT: O(n + m); bộ nhớ O(n + m). Đỉnh 0-based; path hợp lệ có m+1 đỉnh.
// Dùng: EulerPathDirected dg(n); dg.addEdge(u,v); dg.eulerPath();
//       EulerPathUndirected ug(n); ug.addEdge(u,v); ug.eulerCircuit(0);
// Bẫy: đồ thị phải liên thông nếu bỏ các đỉnh cô lập và thỏa điều kiện bậc; cạnh vô hướng dùng edge-id.
// CSES: 1691 1692 1693
struct EulerPathDirected {
    int n, m = 0;
    vector<vector<int>> adj;
    vector<int> indeg, outdeg;
    // Ý nghĩa: khởi tạo đồ thị có hướng n đỉnh (0-based) với danh sách kề rỗng và mảng bậc vào/ra bằng 0.
    // Tham số: n = số đỉnh.
    explicit EulerPathDirected(int n) : n(n), adj(n), indeg(n, 0), outdeg(n, 0) {}
    // Ý nghĩa: thêm cạnh có hướng u->v, cập nhật danh sách kề, outdeg[u], indeg[v] và số cạnh m.
    // Tham số: u = đỉnh đầu; v = đỉnh cuối.
    void addEdge(int u, int v) { adj[u].push_back(v); outdeg[u]++; indeg[v]++; m++; }
    // Ý nghĩa: tìm đường đi/chu trình Euler qua mọi cạnh đúng một lần bằng thuật toán Hierholzer.
    // Trả về: dãy m+1 đỉnh theo thứ tự đi; rỗng nếu không tồn tại (lệch bậc hoặc không liên thông).
    vector<int> eulerPath() {
        int start = -1, cntStart = 0, cntEnd = 0, firstEdge = -1;
        for (int u = 0; u < n; u++) {
            if (outdeg[u] > 0 && firstEdge < 0) firstEdge = u;
            int d = outdeg[u] - indeg[u];
            if (d == 1) { start = u; cntStart++; }
            else if (d == -1) cntEnd++;
            else if (d != 0) return {};              // lệch bậc > 1 -> không có đường Euler
        }
        if (cntStart > 1 || cntEnd > 1) return {};
        if (m == 0) return {};
        if (start == -1) start = firstEdge;          // chu trình Euler: bắt đầu ở đỉnh bất kỳ có cạnh
        vector<int> ptr(n, 0), stk = {start}, path;
        while (!stk.empty()) {
            int u = stk.back();
            if (ptr[u] < (int)adj[u].size()) stk.push_back(adj[u][ptr[u]++]);
            else { path.push_back(u); stk.pop_back(); }
        }
        reverse(path.begin(), path.end());
        if ((int)path.size() != m + 1) return {};    // không dùng hết cạnh -> đồ thị không liên thông
        return path;
    }
};

struct EulerPathUndirected {
    struct EdgeRef {
        int to, id;
    };
    int n, m = 0;
    vector<vector<EdgeRef>> adj;
    vector<int> degree;

    // Ý nghĩa: khởi tạo đồ thị vô hướng n đỉnh, chưa có cạnh.
    explicit EulerPathUndirected(int n) : n(n), adj(n), degree(n, 0) {}

    // Ý nghĩa: thêm một cạnh vô hướng u-v; hai cung kề dùng chung một edge-id.
    // Hỗ trợ đúng cạnh song song và self-loop (self-loop đóng góp 2 vào bậc).
    void addEdge(int u, int v) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        adj[u].push_back({v, m});
        adj[v].push_back({u, m});
        degree[u]++;
        degree[v]++;
        m++;
    }

    // Ý nghĩa: chạy Hierholzer từ start, đánh dấu edge-id để mỗi cạnh vô hướng chỉ dùng một lần.
    // Trả về: dãy đỉnh đã đảo đúng chiều; rỗng nếu không đi được hết m cạnh.
    vector<int> hierholzer(int start) const {
        vector<int> ptr(n, 0), stk = {start}, path;
        vector<char> used(m, false);
        path.reserve(m + 1);
        while (!stk.empty()) {
            int u = stk.back();
            while (ptr[u] < (int)adj[u].size() && used[adj[u][ptr[u]].id])
                ptr[u]++;
            if (ptr[u] == (int)adj[u].size()) {
                path.push_back(u);
                stk.pop_back();
            } else {
                EdgeRef e = adj[u][ptr[u]++];
                used[e.id] = true;
                stk.push_back(e.to);
            }
        }
        if ((int)path.size() != m + 1) return {};
        reverse(path.begin(), path.end());
        return path;
    }

    // Ý nghĩa: tìm đường đi hoặc chu trình Euler vô hướng.
    // Tham số: preferredStart = đỉnh bắt đầu mong muốn, hoặc -1 để tự chọn.
    // Trả về: path m+1 đỉnh; rỗng nếu số đỉnh bậc lẻ không phải 0/2, start sai, hoặc không liên thông.
    vector<int> eulerPath(int preferredStart = -1) const {
        if (m == 0) return {};
        if (preferredStart != -1)
            assert(0 <= preferredStart && preferredStart < n);
        vector<int> odd;
        int firstEdge = -1;
        for (int u = 0; u < n; u++) {
            if (degree[u] > 0 && firstEdge == -1) firstEdge = u;
            if (degree[u] & 1) odd.push_back(u);
        }
        if (!odd.empty() && odd.size() != 2) return {};

        int start;
        if (preferredStart == -1) start = odd.empty() ? firstEdge : odd[0];
        else {
            start = preferredStart;
            if (degree[start] == 0) return {};
            if (odd.size() == 2 && !(degree[start] & 1)) return {};
        }
        return hierholzer(start);
    }

    // Ý nghĩa: tìm CHU TRÌNH Euler bắt đầu/kết thúc đúng tại start (API trực tiếp cho mail-delivery).
    // Trả về: circuit m+1 đỉnh, hoặc rỗng nếu có bậc lẻ / start cô lập / đồ thị không liên thông.
    vector<int> eulerCircuit(int start) const {
        assert(0 <= start && start < n);
        if (m == 0 || degree[start] == 0) return {};
        for (int d : degree)
            if (d & 1) return {};
        vector<int> path = hierholzer(start);
        if (path.empty() || path.front() != start || path.back() != start) return {};
        return path;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ eulerian-path.hpp -o demo && ./demo
int main() {
    EulerPathDirected g(3);
    g.addEdge(0, 1); g.addEdge(1, 2); g.addEdge(2, 0);   // chu trình Euler
    printf("đường đi Euler:");
    for (int v : g.eulerPath()) printf(" %d", v);        // 0 1 2 0
    printf("\n");
    EulerPathUndirected ug(3);
    ug.addEdge(0, 1); ug.addEdge(1, 2); ug.addEdge(2, 0);
    printf("chu trình Euler vô hướng:");
    for (int v : ug.eulerCircuit(0)) printf(" %d", v);
    printf("\n");
    return 0;
}
#endif
