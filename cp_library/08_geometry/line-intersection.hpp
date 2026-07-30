#pragma once
#include <bits/stdc++.h>
using namespace std;
// Line Intersection — giao điểm HAI ĐƯỜNG THẲNG (mỗi đường qua hai điểm nguyên), trả tọa độ thực, O(1).
// Khi dùng: tìm giao điểm hai đường thẳng; dựng đường thẳng qua hai điểm; kiểm song song / trùng.
// ĐPT: O(1); bộ nhớ O(1). Định thức dùng long long (chính xác), tọa độ giao là double (có sai số nhỏ).
// Dùng: bool ok; auto [x, y] = lineIntersection(a1, a2, b1, b2, ok); // ok=false nếu hai đường SONG SONG (hoặc trùng)
// Bẫy: đây là ĐƯỜNG THẲNG (vô hạn), không phải ĐOẠN; ok=false khi song song; tọa độ ~1e9 để định thức không tràn.
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
// Ý nghĩa: tìm giao điểm hai đường thẳng (a1a2) và (b1b2) bằng tỉ số định thức.
// Tham số: a1, a2 = hai điểm trên đường thứ nhất; b1, b2 = hai điểm trên đường thứ hai; ok = tham chiếu nhận true/false.
// Trả về: tọa độ (x, y) kiểu double của giao điểm khi ok=true; ok=false (kèm {0,0}) nếu hai đường song song/trùng.
pair<double, double> lineIntersection(Point a1, Point a2, Point b1, Point b2, bool& ok) {
    long long d1x = a2.x - a1.x, d1y = a2.y - a1.y, d2x = b2.x - b1.x, d2y = b2.y - b1.y;
    long long denom = d1x * d2y - d1y * d2x;
    if (denom == 0) { ok = false; return {0.0, 0.0}; }
    ok = true;
    long long ex = b1.x - a1.x, ey = b1.y - a1.y;
    double t = (double)(ex * d2y - ey * d2x) / (double)denom;   // tham số trên đường thứ nhất
    return {a1.x + t * d1x, a1.y + t * d1y};
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ line-intersection.hpp -o demo && ./demo
int main() {
    bool ok;
    auto [x, y] = lineIntersection({0, 0}, {2, 2}, {0, 2}, {2, 0}, ok);   // giao tại (1,1)
    bool ok2;
    lineIntersection({0, 0}, {1, 1}, {0, 1}, {1, 2}, ok2);                // song song
    printf("giao điểm = (%.1f, %.1f) ; song song? %d\n", x, y, (int)!ok2);   // (1.0, 1.0) ; 1
    return 0;
}
#endif
