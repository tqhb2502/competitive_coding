#pragma once
#include <bits/stdc++.h>
using namespace std;
// Convex Hull (Andrew monotone chain) — bao lồi của tập điểm nguyên theo CCW, O(n log n) (sắp xếp).
// Khi dùng: tìm bao lồi; nền cho đường kính tập điểm, kiểm điểm trong bao, tối ưu trên bao.
// ĐPT: O(n log n); bộ nhớ O(n). Trả các đỉnh bao theo NGƯỢC CHIỀU KIM ĐỒNG HỒ, KHÔNG gồm điểm thẳng hàng trên cạnh.
// Dùng: auto h = convexHull(pts); // h = đỉnh bao lồi (CCW); |h| < 3 nếu mọi điểm thẳng hàng/trùng
// Bẫy: cross <= 0 để loại điểm thẳng hàng (bao chặt); đổi thành < 0 nếu muốn GIỮ điểm trên cạnh; điểm trùng đã khử.
// CSES: 2195
#ifndef CP_GEO_BASE
#define CP_GEO_BASE
struct Point {
    long long x, y;
    Point operator-(const Point& o) const { return {x - o.x, y - o.y}; }
    bool operator<(const Point& o) const { return x != o.x ? x < o.x : y < o.y; }
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};
long long cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
long long cross(const Point& o, const Point& a, const Point& b) { return cross(a - o, b - o); }
long long dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
int sgn(long long v) { return (v > 0) - (v < 0); }
int orient(const Point& a, const Point& b, const Point& c) { return sgn(cross(a, b, c)); }
bool onSegment(const Point& a, const Point& b, const Point& p) {
    return cross(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x)
        && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
#endif
// Ý nghĩa: dựng bao lồi của tập điểm bằng thuật toán chuỗi đơn điệu Andrew (nửa dưới + nửa trên).
// Tham số: p = danh sách điểm (bị sao chép để sắp xếp/khử trùng bên trong).
// Trả về: các đỉnh bao lồi theo CCW (không lặp đỉnh đầu); nếu < 3 điểm phân biệt thì trả chính các điểm đó.
vector<Point> convexHull(vector<Point> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    int n = (int)p.size();
    if (n < 3) return p;
    vector<Point> h(2 * n);
    int k = 0;
    for (int i = 0; i < n; i++) {               // nửa dưới
        while (k >= 2 && cross(h[k - 2], h[k - 1], p[i]) <= 0) k--;
        h[k++] = p[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; i--) {   // nửa trên
        while (k >= t && cross(h[k - 2], h[k - 1], p[i]) <= 0) k--;
        h[k++] = p[i];
    }
    h.resize(k - 1);
    return h;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ convex-hull.hpp -o demo && ./demo
int main() {
    vector<Point> pts = {{0, 0}, {2, 0}, {2, 2}, {0, 2}, {1, 1}};   // ô vuông + tâm
    auto h = convexHull(pts);
    printf("số đỉnh bao lồi = %d\n", (int)h.size());   // 4 (điểm tâm bị loại)
    return 0;
}
#endif
