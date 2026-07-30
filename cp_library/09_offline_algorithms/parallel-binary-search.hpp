#pragma once
#include <bits/stdc++.h>
using namespace std;
// Parallel Binary Search — chặt nhị phân ĐỒNG THỜI mọi truy vấn: mẫu "ngày sớm nhất hai đỉnh liên thông".
// Khi dùng: mỗi truy vấn cần tìm mốc thời gian nhỏ nhất thỏa điều kiện đơn điệu, mà mô phỏng lại từng truy vấn thì chậm.
// ĐPT: O((n + m + q) log m · α(n)); bộ nhớ O(n + m + q). Cạnh thứ d (1-based) xây vào NGÀY d; DSU dựng lại (O(n)) mỗi vòng.
// Dùng: auto ans = earliestConnected(n, edges, queries); // ans[i] = ngày sớm nhất, 0 nếu a==b, -1 nếu không bao giờ
// Bẫy: đây là MẪU (đổi điều kiện kiểm tra để giải bài khác); điều kiện phải ĐƠN ĐIỆU theo thời gian; ngày 1-based.
// CSES: 2101
// Ý nghĩa: với mỗi truy vấn (a, b), tìm NGÀY sớm nhất mà a và b thuộc cùng một thành phần liên thông.
// Tham số: n = số đỉnh (0-based); edges = cạnh theo thứ tự ngày xây (cạnh i xây vào ngày i+1); queries = các cặp (a, b).
// Trả về: mảng ans, ans[i] = ngày sớm nhất (1-based), 0 nếu a == b, -1 nếu không bao giờ liên thông.
vector<int> earliestConnected(int n, const vector<pair<int, int>>& edges, const vector<pair<int, int>>& queries) {
    int m = (int)edges.size(), q = (int)queries.size();
    vector<int> lo(q, 1), hi(q, m + 1);   // đáp án nằm trong [lo, hi]; hi == m+1 nghĩa là "không bao giờ"
    for (int i = 0; i < q; i++) if (queries[i].first == queries[i].second) { lo[i] = 0; hi[i] = 0; }
    vector<int> p(n), sz(n);
    function<int(int)> find = [&](int x) { while (p[x] != x) x = p[x] = p[p[x]]; return x; };
    auto unite = [&](int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b];
    };
    while (true) {
        vector<vector<int>> at(m + 2);
        bool any = false;
        for (int i = 0; i < q; i++) if (lo[i] < hi[i]) { at[lo[i] + (hi[i] - lo[i]) / 2].push_back(i); any = true; }
        if (!any) break;
        iota(p.begin(), p.end(), 0);
        fill(sz.begin(), sz.end(), 1);
        for (int day = 1; day <= m; day++) {
            unite(edges[day - 1].first, edges[day - 1].second);
            for (int qi : at[day]) {
                if (find(queries[qi].first) == find(queries[qi].second)) hi[qi] = day;   // đủ sớm -> thu hẹp phải
                else lo[qi] = day + 1;                                                   // chưa đủ -> thu hẹp trái
            }
        }
    }
    vector<int> ans(q);
    for (int i = 0; i < q; i++) ans[i] = (lo[i] <= m ? lo[i] : -1);
    return ans;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ parallel-binary-search.hpp -o demo && ./demo
int main() {
    // Ngày 1: 0-1 ; ngày 2: 2-3 ; ngày 3: 1-2  -> 0 và 3 liên thông từ ngày 3
    vector<pair<int, int>> edges = {{0, 1}, {2, 3}, {1, 2}};
    vector<pair<int, int>> qs = {{0, 1}, {0, 3}, {0, 4}};
    auto ans = earliestConnected(5, edges, qs);
    printf("ngày sớm nhất: %d %d %d\n", ans[0], ans[1], ans[2]);   // 1 3 -1
    return 0;
}
#endif
