#pragma once
#include <bits/stdc++.h>
using namespace std;
// Point in Polygon — kiểm một điểm nằm TRONG / TRÊN BIÊN / NGOÀI đa giác đơn bằng số vòng (winding number), O(n).
// Khi dùng: truy vấn điểm thuộc đa giác (kể cả đa giác lõm); phân loại điểm so với đa giác đỉnh nguyên.
// ĐPT: O(n) mỗi truy vấn; bộ nhớ O(1). Dùng số học NGUYÊN (không sai số); đa giác đơn, đỉnh theo thứ tự bất kỳ.
// Dùng: int r = pointInPolygon(poly, q); // 0 = ngoài, 1 = trên biên, 2 = trong
// Bẫy: đa giác phải ĐƠN (không tự cắt); dùng winding nên đúng cả đa giác LÕM; điểm trên biên trả 1 (ưu tiên).
// CSES: 2192
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
// Ý nghĩa: xác định vị trí điểm q so với đa giác poly bằng số vòng (winding number) trên tọa độ nguyên.
// Tham số: poly = danh sách đỉnh đa giác đơn; q = điểm cần kiểm.
// Trả về: 0 nếu q ngoài, 1 nếu trên biên, 2 nếu bên trong.
int pointInPolygon(const vector<Point>& poly, const Point& q) {
    int n = (int)poly.size(), wn = 0;
    for (int i = 0; i < n; i++) {
        Point a = poly[i], b = poly[(i + 1) % n];
        if (onSegment(a, b, q)) return 1;
        if (a.y <= q.y) { if (b.y > q.y && cross(a, b, q) > 0) wn++; }
        else { if (b.y <= q.y && cross(a, b, q) < 0) wn--; }
    }
    return wn != 0 ? 2 : 0;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ point-in-polygon.hpp -o demo && ./demo
int main() {
    vector<Point> sq = {{0, 0}, {4, 0}, {4, 4}, {0, 4}};
    printf("(2,2)=%d ; (4,2)=%d ; (5,5)=%d\n",
           pointInPolygon(sq, {2, 2}), pointInPolygon(sq, {4, 2}), pointInPolygon(sq, {5, 5}));   // 2 ; 1 ; 0
    return 0;
}
#endif
