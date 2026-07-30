#pragma once
#include <bits/stdc++.h>
using namespace std;
// Geometry Basics — điểm nguyên + tích có hướng (cross), tích vô hướng (dot), hướng quay (orient), điểm trên đoạn.
// Khi dùng: nền tảng mọi bài hình học nguyên; kiểm thẳng hàng / trái-phải / trên đoạn; dùng chung cho các file khác.
// ĐPT: mọi thao tác O(1); bộ nhớ O(1). Tọa độ NGUYÊN, |x|,|y| tới ~1e9 để cross không tràn long long (~9.2e18).
// Dùng: Point a{0,0},b{1,0},c{0,1}; cross(a,b,c); orient(a,b,c); onSegment(a,b,p);
// Bẫy: tọa độ lớn hơn ~1e9 phải dùng __int128; orient: >0 CCW (trái), <0 CW (phải), 0 thẳng hàng.
// CSES: 2189
// ---- Hình học cơ sở (dùng chung; guard CP_GEO_BASE chống định nghĩa lại khi bundle nhiều file hình học) ----
#ifndef CP_GEO_BASE
#define CP_GEO_BASE
struct Point {                                    // điểm nguyên; |tọa độ| tới ~1e9 để cross không tràn long long
    long long x, y;
    Point operator-(const Point& o) const { return {x - o.x, y - o.y}; }
    bool operator<(const Point& o) const { return x != o.x ? x < o.x : y < o.y; }
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }
};
long long cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }               // tích có hướng a × b
long long cross(const Point& o, const Point& a, const Point& b) { return cross(a - o, b - o); }  // (a-o) × (b-o)
long long dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }                 // tích vô hướng
int sgn(long long v) { return (v > 0) - (v < 0); }                                             // dấu: -1 / 0 / 1
int orient(const Point& a, const Point& b, const Point& c) { return sgn(cross(a, b, c)); }      // >0 CCW, <0 CW, 0 thẳng hàng
bool onSegment(const Point& a, const Point& b, const Point& p) {                                // p nằm trên đoạn [a,b]?
    return cross(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x)
        && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
#endif

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ geometry-basics.hpp -o demo && ./demo
int main() {
    Point a{0, 0}, b{2, 0}, c{1, 1}, d{4, 0};
    printf("orient(a,b,c)=%d ; orient(a,b,d)=%d ; onSegment(a,d,b)=%d\n",
           orient(a, b, c), orient(a, b, d), (int)onSegment(a, d, b));   // 1 ; 0 ; 1
    return 0;
}
#endif
