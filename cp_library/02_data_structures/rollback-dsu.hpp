#pragma once
#include <bits/stdc++.h>
using namespace std;
// Rollback DSU — DSU có UNDO (union by size, KHÔNG nén đường), O(log n) mỗi thao tác.
// Khi dùng: offline dynamic connectivity (segment tree theo thời gian), quay lui trạng thái.
// ĐPT: find/unite O(log n); rollback O(số union đã hoàn tác); bộ nhớ O(n). Index 0-based.
// Dùng: RollbackDSU d(n); int t=d.snapshot(); d.unite(a,b); ...; d.rollback(t); // về đúng mốc t
// Bẫy: TUYỆT ĐỐI không thêm nén đường (find phải O(log n) để undo được); snapshot lấy trước khi unite.
// CSES: 2133
struct RollbackDSU {
    vector<int> p, sz;
    int comp;
    vector<pair<int, int>> hist;  // (root con bị gắn, root cha) để hoàn tác
    // Ý nghĩa: khởi tạo n phần tử rời nhau (0-based), mỗi phần tử là gốc riêng, comp = n.
    // Tham số: n = số phần tử.
    explicit RollbackDSU(int n) : p(n), sz(n, 1), comp(n) { iota(p.begin(), p.end(), 0); }
    // Ý nghĩa: tìm gốc đại diện của tập chứa x bằng cách đi lên chuỗi cha (không nén đường).
    // Tham số: x = phần tử cần tra cứu.
    // Trả về: gốc (root) của tập chứa x.
    int find(int x) const { while (x != p[x]) x = p[x]; return x; }  // không nén đường
    // Ý nghĩa: lấy mốc trạng thái hiện tại (số union đã thực hiện) để dùng cho rollback sau này.
    // Trả về: kích thước lịch sử hist, dùng làm tham số t cho rollback.
    int snapshot() const { return (int)hist.size(); }
    // Ý nghĩa: hợp nhất hai tập chứa a và b (union by size), ghi lịch sử để có thể hoàn tác.
    // Tham số: a, b = hai phần tử bất kỳ.
    // Trả về: true nếu thực sự hợp nhất; false nếu a, b đã cùng tập.
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        hist.push_back({b, a});
        p[b] = a; sz[a] += sz[b]; comp--;
        return true;
    }
    // Ý nghĩa: hoàn tác các union cho tới khi trạng thái về đúng mốc t (khôi phục p, sz, comp).
    // Tham số: t = mốc trạng thái lấy từ snapshot() (số union muốn giữ lại).
    void rollback(int t) {
        while ((int)hist.size() > t) {
            auto [b, a] = hist.back(); hist.pop_back();
            sz[a] -= sz[b]; p[b] = b; comp++;
        }
    }
    // Ý nghĩa: kiểm tra a và b có cùng một tập (liên thông) hay không.
    // Tham số: a, b = hai phần tử bất kỳ.
    // Trả về: true nếu cùng gốc; ngược lại false.
    bool same(int a, int b) const { return find(a) == find(b); }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ rollback-dsu.hpp -o demo && ./demo
int main() {
    RollbackDSU d(5);
    int t = d.snapshot();
    d.unite(0, 1);
    d.unite(1, 2);
    printf("trước rollback: same(0,2)=%d comp=%d\n", d.same(0, 2), d.comp);
    d.rollback(t);
    printf("sau  rollback: same(0,2)=%d comp=%d\n", d.same(0, 2), d.comp);
    return 0;
}
#endif
