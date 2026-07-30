#pragma once
#include <bits/stdc++.h>
using namespace std;
// Minimal Rotation (Booth) — tìm vị trí bắt đầu của phép QUAY nhỏ nhất theo thứ tự từ điển, O(n).
// Khi dùng: chuẩn hóa xâu tuần hoàn (so hai xâu có là quay của nhau), biểu diễn nhỏ nhất của vòng.
// ĐPT: O(n); bộ nhớ O(n) (dùng s + s). Trả về CHỈ SỐ bắt đầu; xâu quay nhỏ nhất = s[i..] + s[..i).
// Dùng: int i = minRotation(s); string best = s.substr(i) + s.substr(0, i);
// Bẫy: trả về chỉ số (0-based), không phải xâu; xâu rỗng trả 0; so sánh theo mã ký tự (phân biệt hoa/thường).
// CSES: 1110
// Ý nghĩa: tìm chỉ số bắt đầu của phép quay có thứ tự từ điển nhỏ nhất của xâu.
// Tham số: s = xâu đầu vào.
// Trả về: chỉ số i (0-based) sao cho s[i..] + s[..i) là phép quay nhỏ nhất.
int minRotation(const string& s) {
    int n = (int)s.size();
    if (n == 0) return 0;
    string t = s + s;
    int i = 0, ans = 0;
    while (i < n) {
        ans = i;
        int j = i + 1, k = i;
        while (j < 2 * n && t[k] <= t[j]) {
            if (t[k] < t[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) i += j - k;
    }
    return ans;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ min-rotation.hpp -o demo && ./demo
int main() {
    string s = "bcabca";
    int i = minRotation(s);
    printf("quay nhỏ nhất của '%s' bắt đầu tại %d = %s\n",
           s.c_str(), i, (s.substr(i) + s.substr(0, i)).c_str());   // 2 = abcabc
    return 0;
}
#endif
