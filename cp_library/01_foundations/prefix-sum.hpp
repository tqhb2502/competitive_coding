#pragma once
#include <bits/stdc++.h>
using namespace std;
// Prefix Sum — tổng đoạn trên mảng tĩnh (không cập nhật).
// Khi dùng: nhiều truy vấn tổng đoạn [l, r), mảng không đổi.
// ĐPT: build O(n); query O(1); bộ nhớ O(n).
// Dùng: PrefixSum ps(a); ps.sum(l, r); // tổng a[l..r-1] (0-based, nửa mở [l, r))
// Bẫy: dùng long long tránh tràn; r là biên phải MỞ; sum(0, n) = tổng cả mảng.
struct PrefixSum {
    vector<long long> pre;
    template <class T>
    explicit PrefixSum(const vector<T>& a) : pre(a.size() + 1, 0) {
        for (size_t i = 0; i < a.size(); i++) pre[i + 1] = pre[i] + (long long)a[i];
    }
    long long sum(int l, int r) const { return pre[r] - pre[l]; }  // [l, r)
};
