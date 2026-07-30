#pragma once
#include <bits/stdc++.h>
using namespace std;
// Closest Pair — khoảng cách nhỏ nhất giữa hai điểm trong mặt phẳng bằng quét dọc + tập theo y, O(n log n).
// Khi dùng: tìm cặp điểm gần nhau nhất; kết quả dùng bình phương khoảng cách (long long, CHÍNH XÁC, không sai số).
// ĐPT: O(n log n); bộ nhớ O(n). Trả BÌNH PHƯƠNG khoảng cách nhỏ nhất; lấy sqrt nếu cần khoảng cách thực.
// Dùng: long long d2 = closestPair(pts); // bình phương khoảng cách nhỏ nhất giữa hai điểm phân biệt
// Bẫy: cần >= 2 điểm (trả LLONG_MAX nếu ít hơn); kết quả là d^2; tọa độ tới ~1e9 để d^2 không tràn long long.
// CSES: 2194
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
// Ý nghĩa: tìm bình phương khoảng cách nhỏ nhất giữa hai điểm bằng quét theo x + tập trượt theo y.
// Tham số: p = danh sách điểm (bị sao chép để sắp xếp bên trong).
// Trả về: bình phương khoảng cách nhỏ nhất giữa hai điểm phân biệt (LLONG_MAX nếu có < 2 điểm).
long long closestPair(vector<Point> p) {
    int n = (int)p.size();
    if (n < 2) return LLONG_MAX;
    sort(p.begin(), p.end(), [](const Point& a, const Point& b) { return a.x < b.x; });
    auto d2 = [](const Point& a, const Point& b) { long long dx = a.x - b.x, dy = a.y - b.y; return dx * dx + dy * dy; };
    set<pair<long long, long long>> box;   // (y, x), sắp theo y
    long long best = LLONG_MAX;
    int j = 0;
    for (int i = 0; i < n; i++) {
        long long d = (long long)sqrt((double)best) + 1;   // cạnh cửa sổ tìm kiếm theo y
        while (j < i && (p[i].x - p[j].x) * (p[i].x - p[j].x) >= best) { box.erase({p[j].y, p[j].x}); j++; }
        auto lo = box.lower_bound({p[i].y - d, LLONG_MIN});
        auto hi = box.upper_bound({p[i].y + d, LLONG_MAX});
        for (auto it = lo; it != hi; ++it) best = min(best, d2(p[i], {it->second, it->first}));
        box.insert({p[i].y, p[i].x});
    }
    return best;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ closest-pair.hpp -o demo && ./demo
int main() {
    vector<Point> pts = {{0, 0}, {5, 5}, {1, 1}, {8, 3}};
    printf("bình phương khoảng cách nhỏ nhất = %lld\n", closestPair(pts));   // (0,0)-(1,1) -> 2
    return 0;
}
#endif
