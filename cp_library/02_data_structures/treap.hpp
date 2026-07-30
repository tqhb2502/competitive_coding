#pragma once
#include <bits/stdc++.h>
using namespace std;
// Treap — cây nhị phân tìm kiếm cân bằng ngẫu nhiên (multiset thống kê thứ tự), O(log n) kỳ vọng.
// Khi dùng: chèn/xóa + hỏi phần tử nhỏ thứ k / đếm số phần tử nhỏ hơn x, khi PBDS không tiện.
// ĐPT: insert/erase/kth/countLess O(log n) kỳ vọng; bộ nhớ O(n).
// Dùng: Treap t; t.insert(x); t.erase(x); t.kth(k); // nhỏ thứ k (1-based); t.countLess(x); t.size();
// Bẫy: cho phép trùng (multiset); kth dùng 1-based; erase(x) chỉ xóa MỘT bản; priority ngẫu nhiên cố định seed.
// CSES: 1749 2163
struct Treap {
    struct Node { long long key; unsigned pri; int sz, l, r; };
    vector<Node> t;  // t[0] = node rỗng (sz = 0)
    int root = 0;
    mt19937 rng{2463534242u};
    // Ý nghĩa: khởi tạo treap rỗng, tạo node canh gác t[0] (sz = 0) làm cây rỗng.
    Treap() { t.push_back({0, 0, 0, 0, 0}); }
    // Ý nghĩa: cấp phát node lá mới mang key, priority ngẫu nhiên, sz = 1.
    // Tham số: key = giá trị khóa của node mới.
    // Trả về: chỉ số của node vừa tạo trong mảng t.
    int nw(long long key) { t.push_back({key, rng(), 1, 0, 0}); return (int)t.size() - 1; }
    // Ý nghĩa: kích thước cây con gốc x (node rỗng trả 0).
    // Tham số: x = chỉ số node.
    // Trả về: số phần tử trong cây con gốc x.
    int sz(int x) const { return t[x].sz; }
    // Ý nghĩa: cập nhật lại sz của node x từ hai con sau khi cấu trúc thay đổi.
    // Tham số: x = chỉ số node cần tính lại sz.
    void upd(int x) { t[x].sz = 1 + t[t[x].l].sz + t[t[x].r].sz; }
    // Ý nghĩa: tách cây con gốc x thành hai treap theo ngưỡng K.
    // Tham số: x = gốc cây con cần tách; K = ngưỡng khóa.
    // Trả về: cặp {gốc cây các key < K, gốc cây các key >= K}.
    pair<int, int> split(int x, long long K) {
        if (!x) return {0, 0};
        if (t[x].key < K) { auto pr = split(t[x].r, K); t[x].r = pr.first; upd(x); return {x, pr.second}; }
        auto pr = split(t[x].l, K); t[x].l = pr.second; upd(x); return {pr.first, x};
    }
    // Ý nghĩa: tách cây con gốc x thành các khóa <= K và các khóa > K, không cần tính K+1.
    // Tham số: x = gốc cây con cần tách; K = ngưỡng khóa (an toàn cả khi K = LLONG_MAX).
    // Trả về: cặp {gốc cây các key <= K, gốc cây các key > K}.
    pair<int, int> splitUpper(int x, long long K) {
        if (!x) return {0, 0};
        if (t[x].key <= K) {
            auto pr = splitUpper(t[x].r, K);
            t[x].r = pr.first;
            upd(x);
            return {x, pr.second};
        }
        auto pr = splitUpper(t[x].l, K);
        t[x].l = pr.second;
        upd(x);
        return {pr.first, x};
    }
    // Ý nghĩa: hợp hai treap a, b (mọi key của a <= mọi key của b) theo thứ tự priority.
    // Tham số: a = gốc cây trái (key nhỏ hơn); b = gốc cây phải (key lớn hơn).
    // Trả về: gốc của treap đã hợp nhất.
    int merge(int a, int b) {
        if (!a || !b) return a | b;
        if (t[a].pri > t[b].pri) { t[a].r = merge(t[a].r, b); upd(a); return a; }
        t[b].l = merge(a, t[b].l); upd(b); return b;
    }
    // Ý nghĩa: chèn thêm một bản của key vào treap (cho phép trùng, kiểu multiset).
    // Tham số: key = giá trị cần chèn.
    void insert(long long key) {
        auto pr = split(root, key);
        root = merge(merge(pr.first, nw(key)), pr.second);
    }
    // Ý nghĩa: xóa MỘT bản của key khỏi treap nếu tồn tại (không đổi nếu không có).
    // Tham số: key = giá trị cần xóa một bản.
    void erase(long long key) {  // xóa một bản của key nếu có
        auto p1 = split(root, key);             // p1.first: < key
        auto p2 = splitUpper(p1.second, key);   // p2.first: == key, không overflow ở LLONG_MAX
        int eq = p2.first;
        if (eq) eq = merge(t[eq].l, t[eq].r);  // bỏ gốc = một bản
        root = merge(p1.first, merge(eq, p2.second));
    }
    // Ý nghĩa: lấy phần tử nhỏ thứ k trong treap (1-based).
    // Tham số: k = thứ hạng cần lấy (giả định 1 <= k <= size()).
    // Trả về: khóa của phần tử nhỏ thứ k, hoặc LLONG_MIN nếu k vượt kích thước.
    long long kth(int k) const {  // nhỏ thứ k, 1-based (giả định 1 <= k <= size())
        int x = root;
        while (x) {
            int ls = sz(t[x].l);
            if (k <= ls) x = t[x].l;
            else if (k == ls + 1) return t[x].key;
            else { k -= ls + 1; x = t[x].r; }
        }
        return LLONG_MIN;
    }
    // Ý nghĩa: đếm số phần tử có khóa nhỏ hơn key (cũng là số thứ tự của key).
    // Tham số: key = ngưỡng so sánh.
    // Trả về: số phần tử < key.
    int countLess(long long key) const {  // số phần tử < key
        int x = root, cnt = 0;
        while (x) {
            if (t[x].key < key) { cnt += sz(t[x].l) + 1; x = t[x].r; }
            else x = t[x].l;
        }
        return cnt;
    }
    // Ý nghĩa: tổng số phần tử hiện có trong treap.
    // Trả về: kích thước cây gốc root.
    int size() const { return sz(root); }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ treap.hpp -o demo && ./demo
int main() {
    Treap t;
    for (int x : {5, 2, 8, 2, 9}) t.insert(x);   // {2,2,5,8,9}
    printf("size=%d  nhỏ thứ 3=%lld  số phần tử < 8=%d\n", t.size(), t.kth(3), t.countLess(8));
    t.erase(2);
    printf("sau khi xóa một số 2: nhỏ thứ 3=%lld\n", t.kth(3));   // {2,5,8,9} -> 8
    return 0;
}
#endif
