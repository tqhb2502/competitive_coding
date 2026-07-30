#pragma once
#include <bits/stdc++.h>
using namespace std;
// Interval Set — duy trì hợp các đoạn nguyên đóng rời nhau; insert/erase và tìm điểm đã phủ đầu/cuối.
// Khi dùng: các đoạn đã thăm trên mỗi hàng/cột, lịch tài nguyên, hợp/xóa range động bằng ordered map.
// ĐPT: O((k+1) log m) cho insert/erase đụng k đoạn; contains/first/last O(log m); bộ nhớ O(m).
// Dùng: IntervalSet<ll> s; s.insert(l,r); s.firstCovered(l,r); s.lastCovered(l,r); s.erase(l,r);
// Bẫy: đoạn ĐÓNG và tọa độ NGUYÊN; insert tự gộp cả đoạn kề nhau; optional rỗng nghĩa là chưa có điểm phủ.
// CSES: 1742

template<class T>
struct IntervalSet {
    static_assert(is_integral<T>::value, "IntervalSet yêu cầu kiểu tọa độ nguyên");
    map<T, T> segments;  // left -> right, rời nhau và không kề nhau

    // Ý nghĩa: kiểm tra point có thuộc hợp đoạn hiện tại hay không.
    // Tham số: point = tọa độ cần hỏi.
    bool contains(T point) const {
        auto it = segments.upper_bound(point);
        return it != segments.begin() && point <= prev(it)->second;
    }
    // Ý nghĩa: tìm điểm đã phủ nhỏ nhất trong đoạn query đóng [left,right].
    // Tham số: left,right = biên query, yêu cầu left<=right.
    // Trả về: điểm nhỏ nhất thuộc cả query và hợp đoạn; nullopt nếu giao rỗng.
    optional<T> firstCovered(T left, T right) const {
        assert(left <= right);
        auto it = segments.upper_bound(left);
        if (it != segments.begin()) {
            auto before = prev(it);
            if (before->second >= left) return left;
        }
        if (it != segments.end() && it->first <= right) return it->first;
        return nullopt;
    }
    // Ý nghĩa: tìm điểm đã phủ lớn nhất trong đoạn query đóng [left,right].
    // Tham số: left,right = biên query, yêu cầu left<=right.
    // Trả về: điểm lớn nhất thuộc cả query và hợp đoạn; nullopt nếu giao rỗng.
    optional<T> lastCovered(T left, T right) const {
        assert(left <= right);
        auto it = segments.upper_bound(right);
        if (it == segments.begin()) return nullopt;
        --it;
        if (it->second < left) return nullopt;
        return min(right, it->second);
    }
    // Ý nghĩa: thêm toàn bộ đoạn [left,right], gộp mọi đoạn giao hoặc kề nó.
    // Tham số: left,right = biên đóng, yêu cầu left<=right.
    void insert(T left, T right) {
        assert(left <= right);
        auto it = segments.lower_bound(left);
        if (it != segments.begin()) {
            auto before = prev(it);
            bool adjacent = before->second != numeric_limits<T>::max()
                         && before->second + 1 == left;
            if (before->second >= left || adjacent) it = before;
        }
        while (it != segments.end()) {
            bool adjacent = right != numeric_limits<T>::max()
                         && it->first == right + 1;
            if (it->first > right && !adjacent) break;
            left = min(left, it->first);
            right = max(right, it->second);
            it = segments.erase(it);
        }
        segments[left] = right;
    }
    // Ý nghĩa: bỏ mọi điểm trong [left,right], có thể chẻ một đoạn thành hai.
    // Tham số: left,right = biên đóng, yêu cầu left<=right.
    void erase(T left, T right) {
        assert(left <= right);
        auto it = segments.upper_bound(left);
        if (it != segments.begin()) --it;
        vector<pair<T, T>> remaining;
        while (it != segments.end()) {
            T begin = it->first, end = it->second;
            if (end < left) {
                ++it;
                continue;
            }
            if (begin > right) break;
            it = segments.erase(it);
            if (begin < left) remaining.push_back({begin, (T)(left - 1)});
            if (end > right) {
                remaining.push_back({(T)(right + 1), end});
                break;
            }
        }
        for (auto interval : remaining) segments.insert(interval);
    }
    // Ý nghĩa: trả view chỉ đọc của map các đoạn chuẩn hóa left->right.
    const map<T, T>& intervals() const { return segments; }
    // Ý nghĩa: xóa mọi đoạn.
    void clear() { segments.clear(); }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ interval-set.hpp -o demo && ./demo
int main() {
    IntervalSet<long long> visited;
    visited.insert(2, 5);
    visited.insert(7, 8);
    visited.insert(6, 6);  // gộp thành [2,8]
    printf("first=%lld last=%lld ", *visited.firstCovered(0, 4), *visited.lastCovered(4, 20));
    visited.erase(4, 6);
    printf("contains(5)=%d, intervals=%zu\n", visited.contains(5), visited.intervals().size());
    return 0;
}
#endif
