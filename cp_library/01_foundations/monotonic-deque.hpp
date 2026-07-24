#pragma once
#include <bits/stdc++.h>
using namespace std;
// Monotonic Deque — min/max trên MỌI cửa sổ độ dài k, O(n).
// Khi dùng: sliding window minimum/maximum (phần tử hết hạn ở đầu cửa sổ).
// ĐPT: O(n) — mỗi index vào/ra deque một lần; bộ nhớ O(k).
// Dùng: auto mn = windowMin(a, k); // mn[i] = min a[i..i+k-1], i in [0, n-k]; windowMax tương tự.
// Bẫy: deque lưu INDEX; loại đầu khi index <= i-k; kết quả có đúng n-k+1 phần tử.
template <class T>
vector<T> windowExtreme(const vector<T>& a, int k, bool wantMin) {
    int n = (int)a.size();
    vector<T> res;
    if (k <= 0 || k > n) return res;
    deque<int> dq;
    for (int i = 0; i < n; i++) {
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        while (!dq.empty() && (wantMin ? a[dq.back()] >= a[i] : a[dq.back()] <= a[i])) dq.pop_back();
        dq.push_back(i);
        if (i >= k - 1) res.push_back(a[dq.front()]);
    }
    return res;
}
template <class T> vector<T> windowMin(const vector<T>& a, int k) { return windowExtreme(a, k, true); }
template <class T> vector<T> windowMax(const vector<T>& a, int k) { return windowExtreme(a, k, false); }
