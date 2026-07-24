#pragma once
#include <bits/stdc++.h>
using namespace std;
// Monotonic Stack — phần tử NHỎ hơn gần nhất bên trái/phải mỗi vị trí, O(n).
// Khi dùng: largest rectangle in histogram, đếm đóng góp theo phần tử chặn đầu tiên.
// ĐPT: O(n) — mỗi index push và pop tối đa một lần.
// Dùng: auto L = prevLess(a, true);  // L[i] = index nhỏ hơn gần nhất bên trái, -1 nếu không có
//        auto R = nextLess(a, true);  // R[i] = index nhỏ hơn gần nhất bên phải,  n nếu không có
//        // strict=true dùng '<'; strict=false dùng '<='. Muốn "lớn hơn": đảo dấu mảng.
// Bẫy: quyết định phía nào giữ phần tử BẰNG (strict) để không đếm trùng đoạn.
template <class T>
vector<int> prevLess(const vector<T>& a, bool strict = true) {
    int n = (int)a.size();
    vector<int> res(n, -1), st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && (strict ? a[st.back()] >= a[i] : a[st.back()] > a[i])) st.pop_back();
        if (!st.empty()) res[i] = st.back();
        st.push_back(i);
    }
    return res;
}
template <class T>
vector<int> nextLess(const vector<T>& a, bool strict = true) {
    int n = (int)a.size();
    vector<int> res(n, n), st;
    for (int i = n - 1; i >= 0; i--) {
        while (!st.empty() && (strict ? a[st.back()] >= a[i] : a[st.back()] > a[i])) st.pop_back();
        if (!st.empty()) res[i] = st.back();
        st.push_back(i);
    }
    return res;
}
