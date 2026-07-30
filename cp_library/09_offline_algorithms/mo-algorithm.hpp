#pragma once
#include <bits/stdc++.h>
using namespace std;
// Mo's Algorithm — trả lời NHIỀU truy vấn đoạn [l, r] OFFLINE bằng cách sắp xếp theo khối, O((n + q) * sqrt(n)).
// Khi dùng: truy vấn đoạn tĩnh mà thêm/bớt MỘT phần tử ở hai đầu là rẻ (đếm phân biệt, tần suất, nghịch thế...).
// ĐPT: O((n + q) sqrt(n) + n log n + q log q) (gồm cả nén giá trị và sắp truy vấn); bộ nhớ O(n + q).
// Dùng: auto ans = moDistinct(a, queries); // ans[i] = số giá trị PHÂN BIỆT trong đoạn [l, r] (đều 0-based, đóng)
// Bẫy: đây là MẪU đếm phân biệt; đổi hàm add/rem để tính đại lượng khác; truy vấn [l, r] BAO GỒM cả hai đầu.
// CSES: 1734
// Ý nghĩa: đếm số giá trị phân biệt trong mỗi đoạn truy vấn bằng thuật toán Mo (mẫu để chỉnh cho bài khác).
// Tham số: a = mảng giá trị BẤT KỲ (âm/tới 1e9 đều được — hàm tự nén bên trong); queries = đoạn (l, r) 0-based, đóng.
// Trả về: mảng ans, ans[i] = số giá trị phân biệt trong đoạn thứ i.
vector<int> moDistinct(const vector<int>& a0, const vector<pair<int, int>>& queries) {
    int n = (int)a0.size(), q = (int)queries.size();
    if (q == 0) return {};
    // Nén giá trị về [0, số_giá_trị_phân_biệt) để bảng đếm chỉ tốn O(n) (giá trị 1e9 hay âm đều an toàn).
    vector<int> vals = a0;
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    vector<int> a(n);
    for (int i = 0; i < n; i++) a[i] = (int)(lower_bound(vals.begin(), vals.end(), a0[i]) - vals.begin());
    int block = max(1, (int)(n / max(1.0, sqrt((double)q))));
    vector<int> ord(q);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int i, int j) {
        int bi = queries[i].first / block, bj = queries[j].first / block;
        if (bi != bj) return bi < bj;
        return (bi & 1) ? queries[i].second > queries[j].second : queries[i].second < queries[j].second;
    });
    vector<int> cnt(vals.size(), 0), ans(q);
    int distinct = 0, curL = 0, curR = -1;
    auto add = [&](int i) { if (cnt[a[i]]++ == 0) distinct++; };
    auto rem = [&](int i) { if (--cnt[a[i]] == 0) distinct--; };
    for (int idx : ord) {
        int L = queries[idx].first, R = queries[idx].second;
        while (curR < R) add(++curR);
        while (curL > L) add(--curL);
        while (curR > R) rem(curR--);
        while (curL < L) rem(curL++);
        ans[idx] = distinct;
    }
    return ans;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ mo-algorithm.hpp -o demo && ./demo
int main() {
    vector<int> a = {1, 1, 2, 1, 3};
    vector<pair<int, int>> qs = {{0, 2}, {1, 4}, {0, 4}};   // [1,1,2] ; [1,2,1,3] ; toàn bộ
    auto ans = moDistinct(a, qs);
    printf("số phân biệt: %d %d %d\n", ans[0], ans[1], ans[2]);   // 2 ; 3 ; 3
    return 0;
}
#endif
