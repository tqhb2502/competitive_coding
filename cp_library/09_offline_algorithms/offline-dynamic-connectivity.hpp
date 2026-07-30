#pragma once
#include <bits/stdc++.h>
using namespace std;
// Offline Dynamic Connectivity — cạnh XUẤT HIỆN / BIẾN MẤT theo thời gian, trả lời "u, v liên thông?" tại các mốc.
// Khi dùng: đồ thị thay đổi (thêm/xóa cạnh) offline; truy vấn liên thông theo thời điểm; đếm thành phần theo thời gian.
// ĐPT: O((E log T + Q) * log n) qua cây phân đoạn theo thời gian + DSU có rollback; bộ nhớ O(E log T + n).
// Dùng: OfflineDynConn dc(n, T); dc.addEdge(u, v, tl, tr); dc.addQuery(t, u, v); auto ans = dc.solve();
// Bẫy: cạnh tồn tại trong NỬA MỞ [tl, tr); DSU KHÔNG nén đường (để rollback); thời gian t trong [0, T).
// CSES: 2133
struct OfflineDynConn {
    int n, T;
    vector<int> p, rnk;
    vector<array<int, 3>> undo;                 // ngăn xếp hoàn tác: (root a, root b, có tăng rank?)
    vector<vector<pair<int, int>>> seg;          // seg[node] = cạnh phủ trọn đoạn thời gian của node
    vector<array<int, 3>> qs;                    // truy vấn (t, u, v)
    vector<char> ans;
    // Ý nghĩa: khởi tạo n đỉnh, T mốc thời gian; DSU và cây phân đoạn theo thời gian rỗng.
    // Tham số: n = số đỉnh; T = số mốc thời gian.
    OfflineDynConn(int n, int T) : n(n), T(T), p(n), rnk(n, 0), seg(4 * max(1, T)) { iota(p.begin(), p.end(), 0); }
    // Ý nghĩa: tìm gốc tập của x (KHÔNG nén đường để còn rollback được).
    // Tham số: x = đỉnh.
    // Trả về: gốc đại diện của tập chứa x.
    int find(int x) { while (p[x] != x) x = p[x]; return x; }
    // Ý nghĩa: hợp hai tập theo rank, ghi lại thao tác để hoàn tác sau.
    // Tham số: a, b = hai đỉnh cần hợp.
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) { undo.push_back({-1, -1, 0}); return; }
        if (rnk[a] < rnk[b]) swap(a, b);
        undo.push_back({a, b, rnk[a] == rnk[b] ? 1 : 0});
        p[b] = a; if (rnk[a] == rnk[b]) rnk[a]++;
    }
    // Ý nghĩa: hoàn tác lần hợp gần nhất (khôi phục DSU về trạng thái trước).
    void rollback() {
        auto e = undo.back(); undo.pop_back();
        if (e[0] == -1) return;
        p[e[1]] = e[1]; if (e[2]) rnk[e[0]]--;
    }
    // Ý nghĩa: thêm cạnh (u, v) tồn tại trong khoảng thời gian nửa mở [tl, tr) vào cây phân đoạn.
    // Tham số: u, v = hai đầu cạnh; tl, tr = khoảng thời gian cạnh tồn tại (nửa mở).
    void addEdge(int u, int v, int tl, int tr) {
        function<void(int, int, int)> rec = [&](int node, int lo, int hi) {
            if (tr <= lo || hi <= tl) return;
            if (tl <= lo && hi <= tr) { seg[node].push_back({u, v}); return; }
            int mid = (lo + hi) / 2;
            rec(2 * node, lo, mid); rec(2 * node + 1, mid, hi);
        };
        rec(1, 0, T);
    }
    // Ý nghĩa: đăng ký truy vấn "u và v có liên thông không" tại thời điểm t.
    // Tham số: t = thời điểm (trong [0, T)); u, v = hai đỉnh.
    void addQuery(int t, int u, int v) { qs.push_back({t, u, v}); }
    // Ý nghĩa: xử lý toàn bộ bằng DFS cây thời gian, hợp cạnh của node rồi rollback khi lui.
    // Trả về: mảng char, phần tử i = 1 nếu truy vấn thứ i liên thông, ngược lại 0.
    vector<char> solve() {
        ans.assign(qs.size(), 0);
        vector<vector<int>> atTime(T);
        for (int i = 0; i < (int)qs.size(); i++) atTime[qs[i][0]].push_back(i);
        function<void(int, int, int)> dfs = [&](int node, int lo, int hi) {
            int cnt = 0;
            for (auto& e : seg[node]) { unite(e.first, e.second); cnt++; }
            if (hi - lo == 1) {
                for (int qi : atTime[lo]) ans[qi] = (find(qs[qi][1]) == find(qs[qi][2]));
            } else {
                int mid = (lo + hi) / 2;
                dfs(2 * node, lo, mid); dfs(2 * node + 1, mid, hi);
            }
            for (int i = 0; i < cnt; i++) rollback();
        };
        if (T > 0) dfs(1, 0, T);
        return ans;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ offline-dynamic-connectivity.hpp -o demo && ./demo
int main() {
    OfflineDynConn dc(4, 3);
    dc.addEdge(0, 1, 0, 3);   // cạnh 0-1 tồn tại suốt [0,3)
    dc.addEdge(1, 2, 1, 2);   // cạnh 1-2 chỉ ở thời điểm 1
    dc.addQuery(0, 0, 2);     // t=0: 0-2? chưa nối -> 0
    dc.addQuery(1, 0, 2);     // t=1: 0-1-2 -> 1
    dc.addQuery(2, 0, 2);     // t=2: mất cạnh 1-2 -> 0
    auto ans = dc.solve();
    printf("liên thông 0-2 tại t=0,1,2: %d %d %d\n", ans[0], ans[1], ans[2]);   // 0 1 0
    return 0;
}
#endif
