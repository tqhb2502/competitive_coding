#pragma once
#include <bits/stdc++.h>
using namespace std;
// Rollback DSU — DSU có UNDO (union by size, KHÔNG nén đường), O(log n) mỗi thao tác.
// Khi dùng: offline dynamic connectivity (segment tree theo thời gian), quay lui trạng thái.
// ĐPT: find/unite O(log n); rollback O(số union đã hoàn tác); bộ nhớ O(n). Index 0-based.
// Dùng: RollbackDSU d(n); int t=d.snapshot(); d.unite(a,b); ...; d.rollback(t); // về đúng mốc t
// Bẫy: TUYỆT ĐỐI không thêm nén đường (find phải O(log n) để undo được); snapshot lấy trước khi unite.
struct RollbackDSU {
    vector<int> p, sz;
    int comp;
    vector<pair<int, int>> hist;  // (root con bị gắn, root cha) để hoàn tác
    explicit RollbackDSU(int n) : p(n), sz(n, 1), comp(n) { iota(p.begin(), p.end(), 0); }
    int find(int x) const { while (x != p[x]) x = p[x]; return x; }  // không nén đường
    int snapshot() const { return (int)hist.size(); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        hist.push_back({b, a});
        p[b] = a; sz[a] += sz[b]; comp--;
        return true;
    }
    void rollback(int t) {
        while ((int)hist.size() > t) {
            auto [b, a] = hist.back(); hist.pop_back();
            sz[a] -= sz[b]; p[b] = b; comp++;
        }
    }
    bool same(int a, int b) const { return find(a) == find(b); }
};
