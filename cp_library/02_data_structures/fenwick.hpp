#pragma once
#include <bits/stdc++.h>
using namespace std;
// Fenwick Tree (BIT) — tổng tiền tố/đoạn, order statistic; kèm FenwickPrefixMax cho prefix max.
// Khi dùng: nhiều update điểm xen truy vấn prefix/đoạn; PrefixMax dùng khi giá trị tại điểm chỉ tăng bằng chmax.
// ĐPT: mỗi update/query O(log n); bộ nhớ O(n).
// Dùng: Fenwick f(n); f.add(i, v); f.sum(r); f.rangeSum(l, r); f.kth(k); // index 0-based
// Bẫy: r/l là biên nửa mở; lowerBound/kth chỉ đúng khi mọi tần suất không âm; API kiểm tra index/range.
// CSES: 1091 1137 1138 1144 1162 1632 1648 1651 1734 1736 1740 1742 1747 1748 1749 2169 2416 2420 3163 3223 3306 3307 3356
struct Fenwick {
    int n;
    vector<long long> t;
    // Ý nghĩa: khởi tạo Fenwick cho mảng n phần tử, cây 1-based khởi 0 (mảng logic toàn 0).
    // Tham số: n = số phần tử của mảng gốc (index 0-based ngoài).
    explicit Fenwick(int n) : n(n), t(max(0, n) + 1, 0) {
        if (n < 0) throw invalid_argument("Fenwick: n âm");
    }
    // Ý nghĩa: cộng thêm v vào phần tử tại vị trí i (cập nhật điểm, tự chuyển i sang 1-based).
    // Tham số: i = vị trí 0-based; v = lượng cộng thêm.
    void add(int i, long long v) {
        if (i < 0 || i >= n) throw out_of_range("Fenwick::add: index ngoài mảng");
        for (++i; i <= n; i += i & -i) t[i] += v;
    }        // a[i] += v
    // Ý nghĩa: tính tổng tiền tố nửa mở [0, r) của mảng gốc.
    // Tham số: r = biên phải (loại trừ), tính các phần tử index 0..r-1.
    // Trả về: tổng a[0] + a[1] + ... + a[r-1].
    long long sum(int r) const {
        if (r < 0 || r > n) throw out_of_range("Fenwick::sum: biên ngoài mảng");
        long long s = 0;
        for (; r > 0; r -= r & -r) s += t[r];
        return s;
    }  // [0, r)
    // Ý nghĩa: tính tổng đoạn nửa mở [l, r) của mảng gốc.
    // Tham số: l = biên trái (bao gồm); r = biên phải (loại trừ).
    // Trả về: tổng a[l] + ... + a[r-1] = sum(r) - sum(l).
    long long rangeSum(int l, int r) const {
        if (l < 0 || l > r || r > n)
            throw out_of_range("Fenwick::rangeSum: đoạn ngoài mảng");
        return sum(r) - sum(l);
    }               // [l, r)
    // Ý nghĩa: binary lifting trên BIT để tìm vị trí đầu tiên có tổng tiền tố BAO GỒM vị trí đó >= target.
    // Tham số: target = tổng tích lũy cần đạt; yêu cầu mọi a[i] >= 0 để các tổng tiền tố không giảm.
    // Trả về: index 0-based nhỏ nhất i sao cho sum(i+1) >= target; n nếu target > sum(n);
    //          trả 0 nếu target <= 0 (với n=0 thì 0 cũng chính là sentinel n).
    int lowerBound(long long target) const {
        if (target <= 0) return 0;
        int pos = 0;
        long long pref = 0;
        int step = 1;
        while (step <= n / 2) step <<= 1;  // lũy thừa 2 lớn nhất <= n, không overflow
        for (; step > 0; step >>= 1) {
            int nxt = pos + step;
            if (nxt <= n && pref + t[nxt] < target) {
                pos = nxt;
                pref += t[nxt];
            }
        }
        return pos;  // pos phần tử đầu có tổng < target -> index cần tìm là pos
    }
    // Ý nghĩa: lấy vị trí của phần tử thứ k trong multiset tần suất lưu trên Fenwick.
    // Tham số: k = thứ hạng 1-based; yêu cầu tần suất không âm.
    // Trả về: index 0-based của phần tử thứ k; n nếu tổng tần suất < k.
    int kth(long long k) const {
        if (k <= 0) throw invalid_argument("Fenwick::kth: k phải >= 1");
        return lowerBound(k);
    }
};

// Fenwick cho phép point-chmax và prefix-max; không cần phép nghịch đảo.
// Dùng: FenwickPrefixMax f(n); f.chmax(i, value); f.prefixMax(r); // max trên [0,r)
// Bẫy: chỉ hỗ trợ cập nhật đơn điệu chmax; không thể giảm/xóa một giá trị đã đưa vào cây.
struct FenwickPrefixMax {
    int n;
    long long identity;
    vector<long long> t;

    // Ý nghĩa: khởi tạo cây prefix-max với phần tử đơn vị (mặc định -vô cực).
    // Tham số: n = số vị trí 0-based; identity = giá trị trả về cho prefix rỗng.
    explicit FenwickPrefixMax(int n, long long identity = LLONG_MIN)
        : n(n), identity(identity), t(max(0, n) + 1, identity) {
        if (n < 0) throw invalid_argument("FenwickPrefixMax: n âm");
    }

    // Ý nghĩa: cập nhật a[index] = max(a[index], value).
    // Tham số: index = vị trí 0-based; value = ứng viên mới.
    void chmax(int index, long long value) {
        if (index < 0 || index >= n)
            throw out_of_range("FenwickPrefixMax: index ngoài mảng");
        for (++index; index <= n; index += index & -index)
            t[index] = std::max(t[index], value);
    }

    // Ý nghĩa: lấy max của các giá trị từng chmax tại vị trí thuộc [0,right).
    // Tham số: right = biên phải nửa mở, 0 <= right <= n.
    // Trả về: prefix max, hoặc identity nếu right == 0/chưa có cập nhật trong prefix.
    long long prefixMax(int right) const {
        if (right < 0 || right > n)
            throw out_of_range("FenwickPrefixMax: biên ngoài mảng");
        long long result = identity;
        for (; right > 0; right -= right & -right)
            result = std::max(result, t[right]);
        return result;
    }
};
// Range-update + point-query : nuôi Fenwick trên mảng hiệu — add(l,+v), add(r,-v); giá trị a[i] = sum(i+1).
// Range-update + range-query : cần hai Fenwick (hoặc dùng lazy-segment-tree cho tổng quát).

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ fenwick.hpp -o demo && ./demo
int main() {
    Fenwick f(5);
    f.add(0, 3);
    f.add(2, 5);
    printf("sum[0,3)=%lld  rangeSum(1,5)=%lld\n", f.sum(3), f.rangeSum(1, 5));
    Fenwick freq(5);
    for (int i : {1, 1, 3, 4}) freq.add(i, 1);  // multiset {1,1,3,4}
    printf("phần tử thứ 3 ở index=%d; lowerBound(prefix>=4)=%d\n",
           freq.kth(3), freq.lowerBound(4));      // 3, 4
    FenwickPrefixMax best(5);
    best.chmax(1, 7);
    best.chmax(3, 4);
    printf("prefixMax[0,4)=%lld\n", best.prefixMax(4));  // 7
    return 0;
}
#endif
