#pragma once
#include <bits/stdc++.h>
using namespace std;
// Tìm chu trình (in ra đỉnh) — dựng lại MỘT chu trình cụ thể cho đồ thị có hướng / vô hướng bằng DFS lặp.
// Khi dùng: đề đòi in ra một "round trip" (dãy đỉnh của chu trình), hoặc kiểm tra đồ thị là DAG / là rừng.
// ĐPT: findCycleDirected O(n + m), findCycleUndirected O(n + m) (mỗi cạnh xét 2 lần); bộ nhớ phụ O(n), danh sách kề O(n + m).
// Dùng: vector<vector<int>> d(n); d[u].push_back(v); auto c1 = findCycleDirected(n, d);
//       vector<vector<pair<int,int>>> g(n); g[u].push_back({v, id}); g[v].push_back({u, id}); auto c2 = findCycleUndirected(n, g);
//       // c = {v1, v2, ..., vk, v1} (đỉnh đầu LẶP LẠI ở cuối), rỗng nếu vô chu trình; CSES in c.size() rồi từng x+1
// Bẫy: đỉnh 0-based (CSES 1-based → cộng 1 khi in); bản vô hướng bỏ cạnh cha theo ID CẠNH (mỗi cạnh 1 id, đẩy 2 chiều) — bỏ theo đỉnh cha sẽ mất chu trình do cạnh song song; kết quả rỗng nghĩa là KHÔNG có chu trình; self-loop cho ra {u, u}, cạnh song song cho ra {u, v, u}.
// CSES: 1669 1678
// Ý nghĩa: tìm một chu trình có hướng bằng DFS LẶP tô màu 0/1/2 (0 chưa thăm, 1 đang trên stack, 2 xong); gặp cạnh tới đỉnh màu 1 là back edge nên truy vết parent để dựng chu trình. Bản lặp nên n = 1e5 không tràn stack.
// Tham số: n = số đỉnh (0-based); adj = danh sách kề CÓ HƯỚNG, adj[u] chứa mọi v có cạnh u -> v (adj.size() phải >= n).
// Trả về: dãy đỉnh chu trình theo đúng chiều cạnh, phần tử đầu lặp lại ở cuối {v1, ..., vk, v1} (kích thước k+1 >= 2); rỗng nếu đồ thị không có chu trình.
vector<int> findCycleDirected(int n, const vector<vector<int>>& adj) {
    vector<char> color(n, 0);
    vector<int> par(n, -1), it(n, 0), stk;
    // Truy vết chu trình từ back edge u -> v: leo parent từ u tới v rồi đảo lại và đóng vòng.
    auto buildCycle = [&](int u, int v) {
        vector<int> cyc;
        for (int x = u; x != v; x = par[x]) cyc.push_back(x);
        cyc.push_back(v);
        reverse(cyc.begin(), cyc.end());
        cyc.push_back(v);                       // đóng vòng: v ... u v
        return cyc;
    };
    for (int s = 0; s < n; s++) {
        if (color[s] != 0) continue;
        color[s] = 1; stk.push_back(s);
        while (!stk.empty()) {
            int u = stk.back();
            if (it[u] < (int)adj[u].size()) {
                int v = adj[u][it[u]++];
                if (color[v] == 0) { color[v] = 1; par[v] = u; stk.push_back(v); }
                else if (color[v] == 1) return buildCycle(u, v);   // v còn trên stack -> là tổ tiên của u
            } else {
                color[u] = 2; stk.pop_back();
            }
        }
    }
    return {};
}

// Ý nghĩa: tìm một chu trình vô hướng bằng DFS LẶP; bỏ qua ĐÚNG cạnh vừa đi vào theo ID CẠNH nên cạnh song song (chu trình độ dài 2) và self-loop vẫn được phát hiện.
// Tham số: n = số đỉnh (0-based); adj = danh sách kề VÔ HƯỚNG, mỗi cạnh thứ id đẩy vào cả adj[u] = {(v, id)} và adj[v] = {(u, id)}; id phải khác nhau giữa các cạnh (adj.size() phải >= n).
// Trả về: dãy đỉnh chu trình theo thứ tự đi, phần tử đầu lặp lại ở cuối {v1, ..., vk, v1} (kích thước k+1 >= 2); rỗng nếu đồ thị là rừng (không có chu trình).
vector<int> findCycleUndirected(int n, const vector<vector<pair<int, int>>>& adj) {
    vector<char> state(n, 0);                   // 0 chưa thăm, 1 đang trên stack, 2 đã xong
    vector<int> par(n, -1), parEdge(n, -1), it(n, 0), stk;
    // Truy vết chu trình từ back edge u - v: leo parent từ u tới v rồi đảo lại và đóng vòng.
    auto buildCycle = [&](int u, int v) {
        vector<int> cyc;
        for (int x = u; x != v; x = par[x]) cyc.push_back(x);
        cyc.push_back(v);
        reverse(cyc.begin(), cyc.end());
        cyc.push_back(v);
        return cyc;
    };
    for (int s = 0; s < n; s++) {
        if (state[s] != 0) continue;
        state[s] = 1; stk.push_back(s);
        while (!stk.empty()) {
            int u = stk.back();
            if (it[u] < (int)adj[u].size()) {
                auto [v, eid] = adj[u][it[u]++];
                if (eid == parEdge[u]) continue;                   // bỏ đúng một lần cạnh vừa đi vào u
                if (state[v] == 0) { state[v] = 1; par[v] = u; parEdge[v] = eid; stk.push_back(v); }
                else if (state[v] == 1) return buildCycle(u, v);   // back edge tới tổ tiên (v == u nếu self-loop)
            } else {
                state[u] = 2; stk.pop_back();
            }
        }
    }
    return {};
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ find-cycle.hpp -o demo && ./demo
int main() {
    // Có hướng: 0->1, 1->2, 2->0 (chu trình), 2->3 (nhánh cụt)
    vector<vector<int>> dg = {{1}, {2}, {0, 3}, {}};
    vector<int> c1 = findCycleDirected(4, dg);
    printf("có hướng:");
    for (int x : c1) printf(" %d", x);           // 0 1 2 0
    printf("\n");

    vector<vector<int>> dag = {{1, 2}, {3}, {3}, {}};
    printf("DAG rỗng? %d\n", (int)findCycleDirected(4, dag).empty());   // 1

    // Vô hướng: tam giác 1-2-3 (id 1,2,3) + cạnh treo 0-1 (id 0)
    vector<vector<pair<int, int>>> ug(4);
    auto link = [&](int u, int v, int id) { ug[u].push_back({v, id}); ug[v].push_back({u, id}); };
    link(0, 1, 0); link(1, 2, 1); link(2, 3, 2); link(3, 1, 3);
    vector<int> c2 = findCycleUndirected(4, ug);
    printf("vô hướng:");
    for (int x : c2) printf(" %d", x);           // 1 2 3 1
    printf("\n");

    // Cạnh song song 0-1 (id 0 và 1) -> chu trình độ dài 2
    vector<vector<pair<int, int>>> par2(2);
    par2[0].push_back({1, 0}); par2[1].push_back({0, 0});
    par2[0].push_back({1, 1}); par2[1].push_back({0, 1});
    vector<int> c3 = findCycleUndirected(2, par2);
    printf("song song: %d đỉnh ->", (int)c3.size());
    for (int x : c3) printf(" %d", x);           // 3 đỉnh -> 0 1 0
    printf("\n");

    vector<vector<pair<int, int>>> tree(3);      // cây 0-1, 1-2 (id 0, 1)
    tree[0].push_back({1, 0}); tree[1].push_back({0, 0});
    tree[1].push_back({2, 1}); tree[2].push_back({1, 1});
    printf("cây rỗng? %d\n", (int)findCycleUndirected(3, tree).empty());   // 1
    return 0;
}
#endif
