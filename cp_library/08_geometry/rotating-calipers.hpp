#pragma once
#include <bits/stdc++.h>
using namespace std;
// Rotating Calipers — đường kính tập điểm (khoảng cách LỚN NHẤT giữa hai điểm) qua bao lồi + kẹp xoay, O(n log n).
// Khi dùng: cặp điểm xa nhau nhất; bề rộng; các bài tối ưu trên hai điểm đối xứng của bao lồi.
// ĐPT: O(n log n) (sắp xếp dựng bao); bộ nhớ O(n). Trả BÌNH PHƯƠNG khoảng cách lớn nhất (long long, chính xác).
// Dùng: long long d2 = diameter2(pts); // bình phương khoảng cách lớn nhất giữa hai điểm
// Bẫy: đỉnh đối xứng luôn nằm trên BAO LỒI nên chỉ cần kẹp trên bao; kết quả là d^2; tọa độ tới ~1e9.
// CSES: —
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
// Ý nghĩa: đường kính (bình phương khoảng cách lớn nhất) của tập điểm, dựng bao lồi rồi kẹp xoay các cặp đối xứng.
// Tham số: pts = danh sách điểm (bị sao chép để dựng bao bên trong).
// Trả về: bình phương khoảng cách lớn nhất giữa hai điểm (0 nếu <= 1 điểm phân biệt).
long long diameter2(vector<Point> pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = (int)pts.size();
    auto d2 = [](const Point& a, const Point& b) { long long dx = a.x - b.x, dy = a.y - b.y; return dx * dx + dy * dy; };
    if (n <= 1) return 0;
    if (n == 2) return d2(pts[0], pts[1]);
    vector<Point> h(2 * n);
    int k = 0;
    for (int i = 0; i < n; i++) { while (k >= 2 && cross(h[k - 2], h[k - 1], pts[i]) <= 0) k--; h[k++] = pts[i]; }
    for (int i = n - 2, t = k + 1; i >= 0; i--) { while (k >= t && cross(h[k - 2], h[k - 1], pts[i]) <= 0) k--; h[k++] = pts[i]; }
    h.resize(k - 1);
    int m = (int)h.size();
    if (m == 1) return 0;
    if (m == 2) return d2(h[0], h[1]);
    long long best = 0;
    for (int i = 0, j = 1; i < m; i++) {
        Point e = h[(i + 1) % m] - h[i];
        while (cross(e, h[(j + 1) % m] - h[i]) > cross(e, h[j] - h[i])) j = (j + 1) % m;   // đỉnh đối xứng của cạnh i
        best = max(best, max(d2(h[i], h[j]), d2(h[(i + 1) % m], h[j])));
    }
    return best;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ rotating-calipers.hpp -o demo && ./demo
int main() {
    vector<Point> pts = {{0, 0}, {3, 0}, {3, 4}, {0, 4}};   // chữ nhật 3×4
    printf("bình phương đường kính = %lld\n", diameter2(pts));   // đường chéo 3-4 -> 25
    return 0;
}
#endif
