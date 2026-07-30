#pragma once
#include <bits/stdc++.h>
using namespace std;
// Polygon Area & Pick — diện tích đa giác (công thức con giày) + số điểm nguyên trên biên / bên trong (định lý Pick).
// Khi dùng: tính diện tích đa giác đơn; đếm điểm nguyên trong/trên đa giác đỉnh nguyên (Pick: A = I + B/2 - 1).
// ĐPT: mọi hàm O(n); bộ nhớ O(1). Trả 2×diện tích (long long, chính xác); |A| = area, dấu > 0 nếu đỉnh theo CCW.
// Dùng: polygonArea2(p); boundaryPoints(p); interiorPoints(p); // p là đa giác đơn, đỉnh nguyên theo thứ tự
// Bẫy: polygonArea2 trả 2A CÓ DẤU (lấy trị tuyệt đối để ra diện tích); Pick chỉ đúng với đỉnh NGUYÊN; đa giác không tự cắt.
// CSES: 2191 2193
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
// Ý nghĩa: 2 lần diện tích CÓ DẤU của đa giác đơn theo công thức con giày (shoelace).
// Tham số: p = danh sách đỉnh đa giác theo thứ tự (CCW cho dấu dương).
// Trả về: 2*A có dấu; |kết quả|/2 = diện tích; dương nếu đỉnh theo CCW.
long long polygonArea2(const vector<Point>& p) {
    long long s = 0;
    int n = (int)p.size();
    for (int i = 0; i < n; i++) s += cross(p[i], p[(i + 1) % n]);
    return s;
}
// Ý nghĩa: số điểm nguyên nằm trên BIÊN đa giác (mỗi cạnh đóng góp gcd(|dx|,|dy|) điểm).
// Tham số: p = danh sách đỉnh nguyên.
// Trả về: số điểm nguyên trên biên.
long long boundaryPoints(const vector<Point>& p) {
    long long b = 0;
    int n = (int)p.size();
    for (int i = 0; i < n; i++) {
        Point d = p[(i + 1) % n] - p[i];
        b += std::gcd(llabs(d.x), llabs(d.y));
    }
    return b;
}
// Ý nghĩa: số điểm nguyên nằm HẲN BÊN TRONG đa giác, suy từ định lý Pick (I = A - B/2 + 1).
// Tham số: p = danh sách đỉnh nguyên.
// Trả về: số điểm nguyên bên trong.
long long interiorPoints(const vector<Point>& p) {
    long long a2 = llabs(polygonArea2(p)), b = boundaryPoints(p);
    return (a2 - b + 2) / 2;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ polygon-area.hpp -o demo && ./demo
int main() {
    vector<Point> p = {{0, 0}, {4, 0}, {4, 3}, {0, 3}};   // chữ nhật 4×3
    printf("2A=%lld (diện tích %lld) ; biên=%lld ; trong=%lld\n",
           polygonArea2(p), llabs(polygonArea2(p)) / 2, boundaryPoints(p), interiorPoints(p));   // 24 (12) ; 14 ; 6
    return 0;
}
#endif
