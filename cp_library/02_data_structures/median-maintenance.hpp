#pragma once
#include <bits/stdc++.h>
using namespace std;
// Median Maintenance — hai multiset cân bằng, thêm/xóa, median dưới và tổng khoảng cách tuyệt đối.
// Khi dùng: median/cost của cửa sổ trượt hoặc multiset động; cần xử lý giá trị trùng.
// ĐPT: add/erase O(log n), median/cost O(1); bộ nhớ O(n).
// Dùng: MedianMaintenance mm; mm.add(x); mm.erase(x); mm.median(); mm.absoluteCost();
// Bẫy: median() là median DƯỚI khi size chẵn; erase trả false nếu không thấy; tổng dùng long long.
// CSES: 1074 1076 1077
struct MedianMaintenance {
    multiset<long long> low, high;  // low chứa ceil(n/2) phần tử nhỏ nhất
    long long sumLow = 0, sumHigh = 0;

    // Ý nghĩa: thêm một giá trị và tái cân bằng hai nửa.
    // Tham số: value = giá trị cần thêm.
    void add(long long value) {
        if (low.empty() || value <= *low.rbegin()) {
            low.insert(value);
            sumLow += value;
        } else {
            high.insert(value);
            sumHigh += value;
        }
        rebalance();
    }

    // Ý nghĩa: xóa đúng một bản của value nếu tồn tại.
    // Tham số: value = giá trị cần xóa.
    // Trả về: true nếu đã xóa, false nếu multiset không chứa value.
    bool erase(long long value) {
        auto itLow = low.find(value);
        if (itLow != low.end()) {
            sumLow -= value;
            low.erase(itLow);
            rebalance();
            return true;
        }
        auto itHigh = high.find(value);
        if (itHigh == high.end()) return false;
        sumHigh -= value;
        high.erase(itHigh);
        rebalance();
        return true;
    }

    // Ý nghĩa: trả median dưới (phần tử hạng (n+1)/2).
    // Trả về: median dưới; ném logic_error nếu cấu trúc rỗng.
    long long median() const {
        if (low.empty()) throw logic_error("MedianMaintenance::median: rỗng");
        return *low.rbegin();
    }

    // Ý nghĩa: tính tổng |x-median| trên toàn multiset.
    // Trả về: tổng khoảng cách tới median dưới (cũng là chi phí L1 tối thiểu).
    long long absoluteCost() const {
        if (low.empty()) return 0;
        long long med = *low.rbegin();
        return med * (long long)low.size() - sumLow
             + sumHigh - med * (long long)high.size();
    }

    // Ý nghĩa: lấy tổng số phần tử đang lưu.
    // Trả về: kích thước hai multiset.
    int size() const { return (int)(low.size() + high.size()); }

private:
    // Ý nghĩa: duy trì |low|=ceil(n/2), |high|=floor(n/2) và max(low)<=min(high).
    void rebalance() {
        while (low.size() < high.size()) {
            auto it = high.begin();
            long long value = *it;
            high.erase(it); sumHigh -= value;
            low.insert(value); sumLow += value;
        }
        while (low.size() > high.size() + 1) {
            auto it = prev(low.end());
            long long value = *it;
            low.erase(it); sumLow -= value;
            high.insert(value); sumHigh += value;
        }
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ median-maintenance.hpp -o demo && ./demo
int main() {
    MedianMaintenance mm;
    for (int x : {2, 4, 3, 5, 8}) mm.add(x);
    printf("median=%lld cost=%lld ", mm.median(), mm.absoluteCost());  // 4, 8
    mm.erase(3);
    printf("sau erase: median=%lld cost=%lld\n", mm.median(), mm.absoluteCost());  // 4, 7
    return 0;
}
#endif
