#pragma once
#include <bits/stdc++.h>
using namespace std;
// Wavelet Matrix — trên mảng TĨNH: giá trị nhỏ thứ k trong đoạn, đếm phần tử nhỏ hơn x trong đoạn, O(log MAX).
// Khi dùng: nhiều truy vấn k-th / rank trên đoạn của mảng không đổi; thay cho merge sort tree / persistent.
// ĐPT: build O(n log MAX); kth/countLess O(log MAX); bộ nhớ O(n log MAX).
// Dùng: WaveletMatrix wm(a); wm.kth(l, r, k); // nhỏ thứ k (1-based) trong [l, r); wm.countLess(l, r, x);
// Bẫy: giá trị phải KHÔNG âm (âm thì cộng offset trước); đoạn nửa mở [l, r); mảng tĩnh (không cập nhật).
//       API dùng assert để bắt l/r/k sai và giá trị âm; hỗ trợ cả INT_MAX mà không dịch bit signed quá miền.
// CSES: 3163
struct WaveletMatrix {
    int n, lg;
    vector<vector<int>> acc;  // acc[h][i] = số bit-0 trong i phần tử đầu ở tầng h
    vector<int> zc;           // zc[h] = tổng số bit-0 ở tầng h (mốc phân hoạch)
    // Ý nghĩa: dựng wavelet matrix từ mảng a; mỗi tầng phân hoạch ổn định theo 1 bit (cao xuống thấp), lưu prefix bit-0.
    // Tham số: a = mảng giá trị không âm (sao chép và bị sắp lại nội bộ khi build).
    explicit WaveletMatrix(vector<int> a) : n((int)a.size()) {
        int mx = 0;
        for (int v : a) {
            assert(v >= 0);
            mx = max(mx, v);
        }
        lg = 1;
        while ((1LL << lg) <= mx) lg++;  // 2^lg > mx; dùng 64-bit khi mx = INT_MAX
        acc.assign(lg, vector<int>(n + 1, 0));
        zc.assign(lg, 0);
        for (int h = 0; h < lg; h++) {
            int bit = lg - 1 - h;
            for (int i = 0; i < n; i++)
                acc[h][i + 1] = acc[h][i] + (((a[i] >> bit) & 1) ? 0 : 1);
            zc[h] = acc[h][n];
            vector<int> zeros, ones;  // phân hoạch ỔN ĐỊNH: bit-0 trước, bit-1 sau
            for (int i = 0; i < n; i++) ((a[i] >> bit) & 1 ? ones : zeros).push_back(a[i]);
            int idx = 0;
            for (int v : zeros) a[idx++] = v;
            for (int v : ones) a[idx++] = v;
        }
    }
    // Ý nghĩa: tìm giá trị nhỏ thứ k trong đoạn nửa mở [l, r) bằng cách đi từng tầng bit.
    // Tham số: l, r = biên đoạn 0-based nửa mở [l, r); k = hạng (1-based, k=1 là nhỏ nhất).
    // Trả về: giá trị đứng thứ k khi sắp tăng các phần tử trong [l, r).
    int kth(int l, int r, int k) const {  // nhỏ thứ k (1-based) trong [l, r)
        assert(0 <= l && l <= r && r <= n);
        assert(1 <= k && k <= r - l);
        k--;  // về 0-based
        int res = 0;
        for (int h = 0; h < lg; h++) {
            int l0 = acc[h][l], r0 = acc[h][r], z = r0 - l0;
            if (k < z) { l = l0; r = r0; }
            else { res |= (1 << (lg - 1 - h)); k -= z; l = zc[h] + (l - l0); r = zc[h] + (r - r0); }
        }
        return res;
    }
    // Ý nghĩa: đếm số phần tử có giá trị nhỏ hơn x trong đoạn nửa mở [l, r).
    // Tham số: l, r = biên đoạn 0-based nửa mở [l, r); x = ngưỡng so sánh.
    // Trả về: số phần tử trong [l, r) có giá trị < x.
    int countLess(int l, int r, int x) const {  // số phần tử < x trong [l, r)
        assert(0 <= l && l <= r && r <= n);
        if (x <= 0) return 0;
        if ((long long)x >= (1LL << lg)) return r - l;
        int cnt = 0;
        for (int h = 0; h < lg; h++) {
            int bit = lg - 1 - h, l0 = acc[h][l], r0 = acc[h][r];
            if ((x >> bit) & 1) { cnt += r0 - l0; l = zc[h] + (l - l0); r = zc[h] + (r - r0); }
            else { l = l0; r = r0; }
        }
        return cnt;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ wavelet-matrix.hpp -o demo && ./demo
int main() {
    vector<int> a = {5, 2, 8, 1, 9, 3};
    WaveletMatrix wm(a);
    printf("nhỏ thứ 2 trong [1,5)=%d  số phần tử < 8 trong [0,6)=%d\n",
           wm.kth(1, 5, 2), wm.countLess(0, 6, 8));   // 2   4
    return 0;
}
#endif
