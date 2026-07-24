#pragma once
#include <bits/stdc++.h>
using namespace std;
// DSU (Union-Find) — hợp nhất / kiểm tra cùng thành phần, ~O(α(n)) mỗi thao tác.
// Khi dùng: connectivity chỉ THÊM cạnh, Kruskal MST, gộp nhóm offline.
// ĐPT: find/unite ~O(α(n)) khấu hao; bộ nhớ O(n). Index 0-based.
// Dùng: DSU d(n); d.unite(a,b); d.same(a,b); d.size(a); d.comp (số thành phần).
// Bẫy: KHÔNG hỗ trợ tách/xóa cạnh — cần undo thì dùng rollback-dsu.
// CSES: 1136 1666 1675 1676 1677 1699 1706 2078 2101 2402 3111 3407 3408 3409
struct DSU {
    vector<int> p, sz;
    int comp;
    explicit DSU(int n) : p(n), sz(n, 1), comp(n) { iota(p.begin(), p.end(), 0); }
    int find(int x) { while (x != p[x]) x = p[x] = p[p[x]]; return x; }  // nén đường
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; comp--;
        return true;
    }
    bool same(int a, int b) { return find(a) == find(b); }
    int size(int x) { return sz[find(x)]; }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ dsu.hpp -o demo && ./demo
int main() {
    DSU d(5);
    d.unite(0, 1);
    d.unite(3, 4);
    printf("same(0,1)=%d  same(0,2)=%d  comp=%d  size(0)=%d\n",
           d.same(0, 1), d.same(0, 2), d.comp, d.size(0));
    return 0;
}
#endif
