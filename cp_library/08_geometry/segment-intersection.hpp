#pragma once
#include <bits/stdc++.h>
using namespace std;
// Segment Intersection — kiểm hai đoạn thẳng có giao nhau không (kể cả chạm đầu mút / chồng lên nhau), O(1).
// Khi dùng: phát hiện hai đoạn cắt nhau; đếm giao điểm (kết hợp quét); kiểm đa giác tự cắt.
// ĐPT: O(1); bộ nhớ O(1). Số học NGUYÊN (không sai số) qua hướng quay orient; xử lý cả trường hợp thẳng hàng chồng lấn.
// Dùng: bool ok = segmentsIntersect(a, b, c, d); // đoạn [a,b] và [c,d] có điểm chung?
// Bẫy: trả true cả khi chỉ CHẠM tại một đầu mút hoặc CHỒNG một phần; tọa độ tới ~1e9 để cross không tràn.
// CSES: 2190
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
// Ý nghĩa: kiểm hai đoạn [a,b] và [c,d] có giao nhau không, dựa trên bốn hướng quay và trường hợp thẳng hàng.
// Tham số: a, b = hai đầu đoạn thứ nhất; c, d = hai đầu đoạn thứ hai.
// Trả về: true nếu hai đoạn có ít nhất một điểm chung (kể cả chạm mút / chồng lấn).
bool segmentsIntersect(Point a, Point b, Point c, Point d) {
    int o1 = orient(a, b, c), o2 = orient(a, b, d), o3 = orient(c, d, a), o4 = orient(c, d, b);
    if (o1 != o2 && o3 != o4) return true;                 // cắt nhau thật sự (mỗi đoạn tách đôi đoạn kia)
    if (o1 == 0 && onSegment(a, b, c)) return true;        // các trường hợp thẳng hàng / chạm mút
    if (o2 == 0 && onSegment(a, b, d)) return true;
    if (o3 == 0 && onSegment(c, d, a)) return true;
    if (o4 == 0 && onSegment(c, d, b)) return true;
    return false;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ segment-intersection.hpp -o demo && ./demo
int main() {
    printf("chéo nhau: %d ; rời (thẳng hàng): %d ; chồng (thẳng hàng): %d ; chạm mút: %d\n",
           (int)segmentsIntersect({0, 0}, {2, 2}, {0, 2}, {2, 0}),   // 1
           (int)segmentsIntersect({0, 0}, {1, 1}, {2, 2}, {3, 3}),   // 0
           (int)segmentsIntersect({0, 0}, {2, 2}, {1, 1}, {3, 3}),   // 1
           (int)segmentsIntersect({0, 0}, {2, 2}, {2, 2}, {3, 5}));  // 1
    return 0;
}
#endif
