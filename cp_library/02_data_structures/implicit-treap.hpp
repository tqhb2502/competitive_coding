#pragma once
#include <bits/stdc++.h>
using namespace std;
// Implicit Treap — mảng động: đảo đoạn (lazy), tổng đoạn, chèn/xóa theo VỊ TRÍ, O(log n) kỳ vọng.
// Khi dùng: cần đảo ngược đoạn [l, r) nhiều lần + hỏi tổng đoạn; hoặc chèn/xóa ở giữa dãy.
// ĐPT: build O(n); reverse/sum O(log n) kỳ vọng; bộ nhớ O(n). Chỉ số 0-based, đoạn nửa mở [l, r).
// Dùng: ImplicitTreap t; t.build(a); t.reverse(l, r); t.sum(l, r); auto v = t.toVector();
// Bẫy: khóa theo VỊ TRÍ (không theo giá trị); nhớ push lazy trước khi đi xuống; dùng long long cho tổng.
// CSES: 2072 2073 2074 2075
struct ImplicitTreap {
    struct Node { long long val, sum; unsigned pri; int sz, l, r; bool rev; };
    vector<Node> t;  // t[0] = node rỗng
    int root = 0;
    mt19937 rng{2463534242u};
    // Ý nghĩa: khởi tạo treap rỗng, đẩy node canh gác t[0] (node null) vào vector.
    ImplicitTreap() { t.push_back({0, 0, 0, 0, 0, 0, false}); }
    // Ý nghĩa: tạo node lá mới mang giá trị v (priority ngẫu nhiên, size 1).
    // Tham số: v = giá trị của node.
    // Trả về: chỉ số của node vừa tạo trong vector t.
    int nw(long long v) { t.push_back({v, v, rng(), 1, 0, 0, false}); return (int)t.size() - 1; }
    // Ý nghĩa: cập nhật lại size và sum của node x từ hai con (pull-up).
    // Tham số: x = chỉ số node cần cập nhật.
    void upd(int x) {
        t[x].sz = 1 + t[t[x].l].sz + t[t[x].r].sz;
        t[x].sum = t[x].val + t[t[x].l].sum + t[t[x].r].sum;
    }
    // Ý nghĩa: đánh dấu lazy đảo ngược lên node x (bật/tắt cờ rev), bỏ qua node null.
    // Tham số: x = chỉ số node cần đánh dấu đảo.
    void applyRev(int x) { if (x) t[x].rev = !t[x].rev; }
    // Ý nghĩa: đẩy lazy rev của x xuống hai con (đổi chỗ con trái/phải rồi lan cờ), xóa cờ ở x.
    // Tham số: x = chỉ số node cần push lazy trước khi đi xuống.
    void push(int x) {
        if (t[x].rev) {
            swap(t[x].l, t[x].r);
            applyRev(t[x].l); applyRev(t[x].r);
            t[x].rev = false;
        }
    }
    // Ý nghĩa: tách cây gốc x theo vị trí thành (cây k phần tử đầu, phần còn lại), đẩy lazy khi đi xuống.
    // Tham số: x = gốc cây con; k = số phần tử đầu đưa về cây trái.
    // Trả về: cặp {gốc cây trái, gốc cây phải}.
    pair<int, int> split(int x, int k) {
        if (!x) return {0, 0};
        push(x);
        if (t[t[x].l].sz >= k) { auto pr = split(t[x].l, k); t[x].l = pr.second; upd(x); return {pr.first, x}; }
        auto pr = split(t[x].r, k - t[t[x].l].sz - 1); t[x].r = pr.first; upd(x); return {x, pr.second};
    }
    // Ý nghĩa: gộp hai cây a (trái) và b (phải) thành một treap theo thứ tự vị trí, tôn trọng priority.
    // Tham số: a = gốc cây trái; b = gốc cây phải.
    // Trả về: gốc cây sau khi gộp.
    int merge(int a, int b) {
        if (!a || !b) return a | b;
        push(a); push(b);
        if (t[a].pri > t[b].pri) { t[a].r = merge(t[a].r, b); upd(a); return a; }
        t[b].l = merge(a, t[b].l); upd(b); return b;
    }
    // Ý nghĩa: dựng treap từ mảng a bằng cách merge lần lượt các node lá, đặt lại root.
    // Tham số: a = mảng giá trị ban đầu (0-based).
    template <class T>
    void build(const vector<T>& a) { root = 0; for (const auto& v : a) root = merge(root, nw((long long)v)); }
    // Ý nghĩa: đảo ngược đoạn nửa mở [l, r) bằng split/applyRev/merge (lazy).
    // Tham số: l, r = biên đoạn nửa mở [l, r) theo vị trí 0-based.
    void reverse(int l, int r) {  // đảo ngược [l, r)
        auto p2 = split(root, r);
        auto p1 = split(p2.first, l);
        applyRev(p1.second);
        root = merge(merge(p1.first, p1.second), p2.second);
    }
    // Ý nghĩa: tính tổng các phần tử trên đoạn nửa mở [l, r) rồi khôi phục cây.
    // Tham số: l, r = biên đoạn nửa mở [l, r) theo vị trí 0-based.
    // Trả về: tổng giá trị trong [l, r).
    long long sum(int l, int r) {  // tổng [l, r)
        auto p2 = split(root, r);
        auto p1 = split(p2.first, l);
        long long res = t[p1.second].sum;
        root = merge(merge(p1.first, p1.second), p2.second);
        return res;
    }
    // Ý nghĩa: duyệt in-order cây con x (push lazy), ghi lần lượt các giá trị vào out.
    // Tham số: x = gốc cây con cần duyệt; out = vector nhận kết quả (bổ sung vào cuối).
    void collect(int x, vector<long long>& out) {
        if (!x) return;
        push(x);
        collect(t[x].l, out);
        out.push_back(t[x].val);
        collect(t[x].r, out);
    }
    // Ý nghĩa: xuất toàn bộ dãy hiện tại (theo thứ tự vị trí) ra vector.
    // Trả về: vector các giá trị của cây theo thứ tự in-order.
    vector<long long> toVector() { vector<long long> out; collect(root, out); return out; }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ implicit-treap.hpp -o demo && ./demo
int main() {
    vector<long long> a = {1, 2, 3, 4, 5};
    ImplicitTreap t;
    t.build(a);
    t.reverse(1, 4);   // đảo ngược [1,4): 2 3 4 -> 4 3 2
    printf("tổng [0,5)=%lld  mảng: ", t.sum(0, 5));
    for (long long x : t.toVector()) printf("%lld ", x);   // 1 4 3 2 5
    printf("\n");
    return 0;
}
#endif
