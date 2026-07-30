#pragma once
#include <bits/stdc++.h>
using namespace std;
// Monotonic Stack — phần tử NHỎ hơn gần nhất bên trái/phải mỗi vị trí, O(n).
// Khi dùng: largest rectangle in histogram, đếm đóng góp theo phần tử chặn đầu tiên.
// ĐPT: O(n) — mỗi index push và pop tối đa một lần.
// Dùng: auto L = prevLess(a, true);  // L[i] = index nhỏ hơn gần nhất bên trái, -1 nếu không có
//        auto R = nextLess(a, true);  // R[i] = index nhỏ hơn gần nhất bên phải,  n nếu không có
//        strict=true dùng '<'; strict=false dùng '<='. Muốn "lớn hơn": đảo dấu mảng.
// Bẫy: quyết định phía nào giữ phần tử BẰNG (strict) để không đếm trùng đoạn.
// CSES: 1142 1147 1148 1645 2416 3227 3304 3314 3414
// Ý nghĩa: với mỗi i, tìm index của phần tử nhỏ hơn gần nhất bên TRÁI (0-based).
// Tham số: a = mảng đầu vào; strict = true dùng '<' (chặn phần tử bằng), false dùng '<='.
// Trả về: res[i] = index nhỏ hơn gần nhất bên trái, hoặc -1 nếu không có.
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
// Ý nghĩa: với mỗi i, tìm index của phần tử nhỏ hơn gần nhất bên PHẢI (0-based).
// Tham số: a = mảng đầu vào; strict = true dùng '<' (chặn phần tử bằng), false dùng '<='.
// Trả về: res[i] = index nhỏ hơn gần nhất bên phải, hoặc n nếu không có.
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

#ifdef CP_DEMO  // chạy thử: g++ -std=c++17 -DCP_DEMO -x c++ monotonic-stack.hpp -o demo && ./demo
int main() {
    vector<int> a = {2, 1, 4, 3, 3};
    vector<int> L = prevLess(a, true);   // index nhỏ hơn gần nhất bên TRÁI (strict), -1 nếu không có
    vector<int> R = nextLess(a, true);   // ... bên PHẢI, = n nếu không có
    int n = (int)a.size();
    for (int i = 0; i < n; i++) {
        int right = (R[i] == n) ? -1 : R[i];
        // Đoạn tối đa mà a[i] là min ngặt: (prevLess, nextLess) -> độ rộng R[i]-L[i]-1
        printf("a[%d]=%d  prevLess=%d  nextLess=%d  width(a[i] la min)=%d\n",
               i, a[i], L[i], right, R[i] - L[i] - 1);
    }
    return 0;
}
#endif
