// Khung hình học — DÙNG nguyên hàm chung từ snippet, KHÔNG định nghĩa lại Point/cross/orient.
// (Định nghĩa lại sẽ XUNG ĐỘT với khối guard CP_GEO_BASE khi include/bundle snippet 08_geometry:
//  g++ báo "redefinition of struct Point" — lỗi rất dễ mắc lúc đang thi.)
// Biên dịch: g++ -std=c++17 -O2 -Wall -Wextra -I <thư mục gốc repo> geometry.cpp -o sol
#include <bits/stdc++.h>
using namespace std;
#include "cp_library/08_geometry/geometry-basics.hpp"      // Point, cross, dot, sgn, orient, onSegment
// #include "cp_library/08_geometry/convex-hull.hpp"       // thêm snippet hình học khác nếu cần
// #include "cp_library/08_geometry/point-in-polygon.hpp"

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n) || n <= 0) return 0;
    vector<Point> p(n);
    for (auto& q : p) cin >> q.x >> q.y;
    // TODO: thuật toán hình học ở đây (orient/cross/onSegment đã có; tọa độ nguyên tới ~1e9).
    cout << orient(p[0], p[n / 2], p[n - 1]) << '\n';
    return 0;
}
