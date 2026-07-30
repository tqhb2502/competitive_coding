#pragma once
#include <bits/stdc++.h>
using namespace std;
// Manhattan Distance — đường kính online qua x+y/x-y và tổng khoảng cách mọi cặp bằng sort + prefix sum.
// Khi dùng: thêm điểm rồi hỏi khoảng cách L1 lớn nhất; tính tổng |dx|+|dy| trên mọi cặp điểm.
// ĐPT: online add/diameter O(1), bộ nhớ O(1); pair sum O(n log n), bộ nhớ O(n).
// Dùng: OnlineManhattanDiameter d; d.add(p); d.diameter(); __int128 total=manhattanPairDistanceSum(points);
// Bẫy: x+y, x-y và đường kính phải vừa long long; tổng cặp trả __int128; mỗi cặp không thứ tự đếm một lần.
// CSES: 3410 3411
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

struct OnlineManhattanDiameter {
    bool has_point;
    long long min_sum, max_sum, min_difference, max_difference;

    // Ý nghĩa: tạo một tập điểm rỗng có đường kính quy ước bằng 0.
    OnlineManhattanDiameter()
        : has_point(false), min_sum(0), max_sum(0), min_difference(0), max_difference(0) {}

    // Ý nghĩa: thêm một điểm và cập nhật các cực trị của x+y, x-y.
    // Tham số: point = điểm nguyên cần thêm.
    void add(const Point& point) {
        long long sum = point.x + point.y;
        long long difference = point.x - point.y;
        if (!has_point) {
            min_sum = max_sum = sum;
            min_difference = max_difference = difference;
            has_point = true;
            return;
        }
        min_sum = min(min_sum, sum);
        max_sum = max(max_sum, sum);
        min_difference = min(min_difference, difference);
        max_difference = max(max_difference, difference);
    }

    // Ý nghĩa: lấy khoảng cách Manhattan lớn nhất giữa hai điểm đã thêm.
    // Trả về: đường kính L1; bằng 0 nếu tập có ít hơn hai điểm.
    long long diameter() const {
        if (!has_point) return 0;
        return max(max_sum - min_sum, max_difference - min_difference);
    }
};

// Ý nghĩa: tính tổng |a_i-a_j| trên mọi cặp chỉ số i<j.
// Tham số: values = dãy số (sao chép để sắp xếp).
// Trả về: tổng chính xác bằng __int128.
__int128 absolutePairDifferenceSum(vector<long long> values) {
    sort(values.begin(), values.end());
    __int128 prefix = 0, answer = 0;
    for (int i = 0; i < (int)values.size(); i++) {
        answer += (__int128)values[i] * i - prefix;
        prefix += values[i];
    }
    return answer;
}

// Ý nghĩa: tính tổng khoảng cách Manhattan trên mọi cặp điểm không thứ tự.
// Tham số: points = danh sách điểm nguyên.
// Trả về: tổng_{i<j} (|x_i-x_j|+|y_i-y_j|), kiểu __int128.
__int128 manhattanPairDistanceSum(const vector<Point>& points) {
    vector<long long> x, y;
    x.reserve(points.size());
    y.reserve(points.size());
    for (const Point& point : points) {
        x.push_back(point.x);
        y.push_back(point.y);
    }
    return absolutePairDifferenceSum(std::move(x)) + absolutePairDifferenceSum(std::move(y));
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ manhattan-distance.hpp -o demo && ./demo
int main() {
    vector<Point> points = {{0, 0}, {2, 1}, {-1, 3}};
    OnlineManhattanDiameter online;
    for (const Point& point : points) {
        online.add(point);
        printf("%lld ", online.diameter());  // 0 3 5
    }
    printf("\ntổng mọi cặp = %lld\n", (long long)manhattanPairDistanceSum(points));  // 12
    return 0;
}
#endif
