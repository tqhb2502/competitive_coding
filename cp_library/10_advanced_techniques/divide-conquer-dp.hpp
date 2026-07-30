#pragma once
#include <bits/stdc++.h>
using namespace std;
// Divide & Conquer DP — tối ưu dp[j][i] = min(dp[j-1][p] + cost(p, i)) khi điểm tối ưu ĐƠN ĐIỆU, O(k·n log n).
// Khi dùng: chia mảng thành ĐÚNG k đoạn liên tiếp, chi phí mỗi đoạn cho bởi cost(l, r) thỏa bất đẳng thức tứ giác.
// ĐPT: O(k·n log n) lần gọi cost; bộ nhớ O(n). cost(l, r) là chi phí đoạn NỬA MỞ [l, r) (phần tử l..r-1).
// Dùng: long long best = divideConquerDP(n, k, cost); // cost là hàm/lambda (int l, int r) -> long long
// Bẫy: cost PHẢI thỏa tính đơn điệu của opt (vd tổng bình phương, tổng khoảng cách tới trung vị); k <= n; trả 4e18 nếu vô nghiệm.
// CSES: 2086 2087
// Ý nghĩa: chi phí nhỏ nhất khi chia n phần tử thành ĐÚNG k đoạn liên tiếp, dùng chia để trị trên điểm tối ưu.
// Tham số: n = số phần tử; k = số đoạn; cost = hàm chi phí đoạn nửa mở [l, r).
// Trả về: tổng chi phí nhỏ nhất (4e18 nếu không chia được, ví dụ k > n).
template <class Cost>
long long divideConquerDP(int n, int k, Cost cost) {
    const long long INF = (long long)4e18;
    if (k <= 0 || k > n) return INF;
    vector<long long> prv(n + 1, INF), cur(n + 1, INF);
    prv[0] = 0;                                   // 0 đoạn phủ 0 phần tử
    for (int j = 1; j <= k; j++) {
        fill(cur.begin(), cur.end(), INF);
        // Tính cur[i] cho i thuộc [lo, hi], biết điểm cắt tối ưu nằm trong [optlo, opthi].
        function<void(int, int, int, int)> compute = [&](int lo, int hi, int optlo, int opthi) {
            if (lo > hi) return;
            int mid = (lo + hi) / 2, bestP = -1;
            long long best = INF;
            for (int p = optlo; p <= min(mid - 1, opthi); p++) {
                if (prv[p] == INF) continue;
                long long v = prv[p] + cost(p, mid);
                if (v < best) { best = v; bestP = p; }
            }
            cur[mid] = best;
            if (bestP < 0) bestP = optlo;
            compute(lo, mid - 1, optlo, bestP);
            compute(mid + 1, hi, bestP, opthi);
        };
        compute(j, n, j - 1, n - 1);
        swap(prv, cur);
    }
    return prv[n];
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ divide-conquer-dp.hpp -o demo && ./demo
int main() {
    vector<long long> a = {1, 2, 3, 4};
    vector<long long> pre(a.size() + 1, 0);
    for (size_t i = 0; i < a.size(); i++) pre[i + 1] = pre[i] + a[i];
    auto cost = [&](int l, int r) { long long s = pre[r] - pre[l]; return s * s; };   // bình phương tổng đoạn
    printf("chia {1,2,3,4} thành 2 đoạn, tổng bình phương nhỏ nhất = %lld\n",
           divideConquerDP(4, 2, cost));   // {1,2,3}|{4} -> 36 + 16 = 52
    return 0;
}
#endif
