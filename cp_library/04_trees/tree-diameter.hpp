#pragma once
#include <bits/stdc++.h>
using namespace std;
// Đường kính cây — đường đi DÀI NHẤT giữa hai đỉnh trên CÂY (có trọng số), bằng hai lần DFS, O(n).
// Khi dùng: tìm độ dài + hai đầu mút đường kính; đỉnh xa nhất từ một đỉnh; nền cho nhiều bài cây.
// ĐPT: O(n); bộ nhớ O(n). Đỉnh 0-based; adj[u] = {(v, w)} trọng số long long >= 0; DFS LẶP (không tràn stack).
// Dùng: auto [len, a, b] = treeDiameter(adj); // len = độ dài đường kính, a-b là hai đầu mút
// Bẫy: chỉ đúng trên CÂY (liên thông, không chu trình) với trọng số KHÔNG âm; cây rỗng/n=1 -> len = 0.
// CSES: 1131 1132
// Ý nghĩa: tìm đường kính cây bằng hai lần DFS (đỉnh xa nhất từ 0, rồi từ đỉnh đó).
// Tham số: adj = danh sách kề cây vô hướng (0-based), adj[u] = {(v, w)} với w trọng số long long >= 0.
// Trả về: tuple (len, a, b) — len = độ dài đường kính; a, b = hai đầu mút; cây rỗng -> {0, 0, 0}.
tuple<long long, int, int> treeDiameter(const vector<vector<pair<int, long long>>>& adj) {
    int n = (int)adj.size();
    auto farthest = [&](int s) {
        vector<long long> d(n, -1);
        d[s] = 0;
        vector<int> st = {s};
        long long bd = 0; int bn = s;
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            if (d[u] > bd) { bd = d[u]; bn = u; }
            for (auto [v, w] : adj[u]) if (d[v] < 0) { d[v] = d[u] + w; st.push_back(v); }
        }
        return pair<long long, int>{bd, bn};
    };
    if (n == 0) return {0, 0, 0};
    int a = farthest(0).second;
    auto [len, b] = farthest(a);
    return {len, a, b};
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ tree-diameter.hpp -o demo && ./demo
int main() {
    //  0 --1-- 1 --3-- 2
    //          |2
    //          3
    vector<vector<pair<int, long long>>> adj(4);
    auto add = [&](int u, int v, long long w) { adj[u].push_back({v, w}); adj[v].push_back({u, w}); };
    add(0, 1, 1); add(1, 2, 3); add(1, 3, 2);
    auto [len, a, b] = treeDiameter(adj);
    printf("đường kính = %lld (giữa %d và %d)\n", len, min(a, b), max(a, b));   // 5 (giữa 2 và 3): 2-1-3 = 3+2
    return 0;
}
#endif
