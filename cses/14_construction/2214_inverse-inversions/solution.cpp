#include <bits/stdc++.h>
using namespace std;

int main() {
    long long n, k;
    // Đọc n và số nghịch thế cần đạt k
    if (scanf("%lld %lld", &n, &k) != 2) return 0;

    vector<int> res;
    res.reserve((size_t)n);

    // Tham lam từ trái sang phải; các giá trị chưa dùng luôn là đoạn liên tục [lo, hi]
    long long lo = 1, hi = n;
    while (lo <= hi) {
        if (k <= 0) {
            // Xả phần còn lại theo thứ tự tăng dần (thêm 0 nghịch thế)
            for (long long v = lo; v <= hi; ++v) res.push_back((int)v);
            break;
        }
        long long m = hi - lo + 1;      // số giá trị chưa dùng
        if (k >= m - 1) {
            // Đặt giá trị lớn nhất -> tạo thêm đúng m-1 nghịch thế
            res.push_back((int)hi);
            hi--;
            k -= (m - 1);
        } else {
            // 0 < k < m-1: đặt lo+k để đóng góp đúng k nghịch thế,
            // phần còn lại xếp tăng dần (trừ giá trị đã dùng)
            long long chosen = lo + k;
            res.push_back((int)chosen);
            for (long long v = lo; v <= hi; ++v)
                if (v != chosen) res.push_back((int)v);
            k = 0;
            break;
        }
    }

    // Xuất kết quả với buffer nhanh
    string out;
    out.reserve(res.size() * 7 + 1);
    char buf[16];
    for (size_t i = 0; i < res.size(); ++i) {
        if (i) out.push_back(' ');
        int x = res[i];
        int len = 0;
        if (x == 0) buf[len++] = '0';
        while (x > 0) { buf[len++] = char('0' + x % 10); x /= 10; }
        while (len > 0) out.push_back(buf[--len]);
    }
    out.push_back('\n');
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
