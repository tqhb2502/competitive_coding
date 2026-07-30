#pragma once
#include <bits/stdc++.h>
using namespace std;
// Cầu & Khớp — tìm cầu (cạnh) và khớp (đỉnh) của đồ thị VÔ HƯỚNG bằng low-link lặp, O(n + m).
// Khi dùng: cạnh/đỉnh mà bỏ đi làm đồ thị mất liên thông; xử lý mạng đường, thành phần 2-cạnh/2-đỉnh.
// ĐPT: O(n + m); bộ nhớ O(n + m). Đỉnh 0-based; nhận cạnh SONG SONG đúng (theo id cạnh, không theo đỉnh cha).
// Dùng: BridgeArtic g(n); g.addEdge(u, v); g.build(); g.bridges (cặp đỉnh); g.isArt[v] (1 nếu là khớp).
// Bẫy: đồ thị VÔ HƯỚNG; addEdge một lần cho mỗi cạnh; khuyên (self-loop) không là cầu; bản lặp không tràn stack.
// CSES: 1705 2076 2077 2177
struct BridgeArtic {
    int n, m = 0, timer = 0;
    vector<vector<pair<int, int>>> adj;   // adj[u] = {(v, id cạnh)}
    vector<int> num, low;
    vector<char> isArt;
    vector<pair<int, int>> bridges;
    // Ý nghĩa: khởi tạo đồ thị n đỉnh (0-based) với danh sách kề rỗng và các mảng low-link chưa thăm.
    // Tham số: n = số đỉnh.
    explicit BridgeArtic(int n) : n(n), adj(n), num(n, -1), low(n, -1), isArt(n, 0) {}
    // Ý nghĩa: thêm một cạnh vô hướng u-v, gán cùng một id cạnh cho cả hai chiều rồi tăng bộ đếm cạnh m.
    // Tham số: u, v = hai đầu mút của cạnh (0-based).
    void addEdge(int u, int v) { adj[u].push_back({v, m}); adj[v].push_back({u, m}); m++; }
    // Ý nghĩa: chạy DFS low-link lặp trên mọi thành phần liên thông, điền bridges (danh sách cầu) và isArt (khớp).
    void build() {
        vector<int> it(n, 0), parEdge(n, -1), stk;
        for (int s = 0; s < n; s++) {
            if (num[s] != -1) continue;
            stk.push_back(s);
            int rootKids = 0;
            while (!stk.empty()) {
                int u = stk.back();
                if (it[u] == 0) { num[u] = low[u] = timer++; }
                if (it[u] < (int)adj[u].size()) {
                    auto [v, eid] = adj[u][it[u]++];
                    if (eid == parEdge[u]) continue;      // bỏ đúng cạnh vừa đi vào (giữ cạnh song song còn lại)
                    if (num[v] == -1) { parEdge[v] = eid; if (u == s) rootKids++; stk.push_back(v); }
                    else low[u] = min(low[u], num[v]);
                } else {
                    stk.pop_back();
                    if (!stk.empty()) {
                        int p = stk.back();
                        low[p] = min(low[p], low[u]);
                        if (low[u] > num[p]) bridges.push_back({p, u});   // cầu p-u
                        if (p != s && low[u] >= num[p]) isArt[p] = 1;     // khớp không phải gốc
                    }
                }
            }
            if (rootKids > 1) isArt[s] = 1;   // gốc là khớp nếu có >= 2 nhánh con
        }
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ bridges-articulation.hpp -o demo && ./demo
int main() {
    BridgeArtic g(5);
    g.addEdge(0, 1); g.addEdge(1, 2); g.addEdge(2, 0);   // tam giác {0,1,2}: không cầu
    g.addEdge(2, 3); g.addEdge(3, 4);                    // đuôi: cầu 2-3, 3-4
    g.build();
    int arts = 0; for (int v = 0; v < 5; v++) arts += g.isArt[v];
    printf("số cầu = %d ; số khớp = %d\n", (int)g.bridges.size(), arts);   // 2 ; 2
    return 0;
}
#endif
