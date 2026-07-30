#pragma once
#include <bits/stdc++.h>
using namespace std;
// CDQ Divide & Conquer — đếm thứ tự bộ phận BA CHIỀU (thời gian, x, y) offline: chia để trị + Fenwick, O(n log^2 n).
// Khi dùng: với mỗi i cần đếm số j ĐỨNG TRƯỚC mà x_j <= x_i và y_j <= y_i (bộ ba đơn điệu, đếm điểm bị trội).
// ĐPT: O(n log^2 n); bộ nhớ O(n). Chiều thứ nhất = THỨ TỰ trong mảng (thời gian), hai chiều còn lại là x và y.
// Dùng: auto ans = cdqDominance(pts); // ans[i] = số j < i thỏa x_j <= x_i và y_j <= y_i
// Bẫy: chiều thời gian là chỉ số mảng (0-based); so sánh KHÔNG NGẶT (<=) ở cả x và y; y được nén nên giá trị tùy ý.
// CSES: —
// Ý nghĩa: với mỗi phần tử, đếm số phần tử đứng trước nó bị trội cả hai chiều (x <= và y <=), bằng CDQ + Fenwick.
// Tham số: pts = danh sách (x, y) theo thứ tự thời gian (chỉ số mảng).
// Trả về: mảng ans, ans[i] = số j < i với x_j <= x_i và y_j <= y_i.
vector<long long> cdqDominance(const vector<pair<int, int>>& pts) {
    int n = (int)pts.size();
    vector<long long> ans(n, 0);
    if (n == 0) return ans;
    vector<int> ys(n);
    for (int i = 0; i < n; i++) ys[i] = pts[i].second;
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int m = (int)ys.size();
    vector<int> bit(m + 1, 0);
    auto yid = [&](int y) { return (int)(lower_bound(ys.begin(), ys.end(), y) - ys.begin()) + 1; };   // 1-based
    auto bitAdd = [&](int i, int v) { for (; i <= m; i += i & -i) bit[i] += v; };
    auto bitSum = [&](int i) { int s = 0; for (; i > 0; i -= i & -i) s += bit[i]; return s; };
    auto byX = [&](int a, int b) {
        return pts[a].first != pts[b].first ? pts[a].first < pts[b].first : pts[a].second < pts[b].second;
    };
    // Chỉ sắp xếp BẢN SAO cục bộ nên thứ tự thời gian của mảng gốc không bị phá.
    function<void(int, int)> cdq = [&](int lo, int hi) {   // xử lý mọi cặp (j, i) với lo <= j < i < hi
        if (hi - lo <= 1) return;
        int mid = (lo + hi) / 2;
        cdq(lo, mid); cdq(mid, hi);
        vector<int> L, R;
        for (int i = lo; i < mid; i++) L.push_back(i);
        for (int i = mid; i < hi; i++) R.push_back(i);
        sort(L.begin(), L.end(), byX);
        sort(R.begin(), R.end(), byX);
        size_t p = 0;
        for (int r : R) {   // thêm dần các điểm nửa trái có x <= x_r rồi hỏi Fenwick theo y
            while (p < L.size() && pts[L[p]].first <= pts[r].first) { bitAdd(yid(pts[L[p]].second), 1); p++; }
            ans[r] += bitSum(yid(pts[r].second));
        }
        for (size_t i = 0; i < p; i++) bitAdd(yid(pts[L[i]].second), -1);   // hoàn tác Fenwick
    };
    cdq(0, n);
    return ans;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ cdq-divide-conquer.hpp -o demo && ./demo
int main() {
    vector<pair<int, int>> pts = {{1, 3}, {2, 2}, {3, 1}, {2, 3}};
    auto ans = cdqDominance(pts);
    printf("số điểm trước bị trội:");
    for (long long v : ans) printf(" %lld", v);   // 0 0 0 2
    printf("\n");
    return 0;
}
#endif
