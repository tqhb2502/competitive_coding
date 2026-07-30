#pragma once
#include <bits/stdc++.h>
using namespace std;
// KMP (prefix function) — hàm tiền tố + tìm mẫu trong văn bản, O(n + m); cũng cho biên (border) và chu kỳ.
// Khi dùng: đếm/định vị mọi lần xuất hiện của mẫu; tìm biên dài nhất; chu kỳ nhỏ nhất của xâu.
// ĐPT: prefixFunction O(n), kmpSearch O(|t| + |p|); bộ nhớ O(n). Biên dài nhất = pi.back(); chu kỳ nhỏ nhất = n - pi.back().
// Dùng: auto pi = prefixFunction(s); auto pos = kmpSearch(t, p); // pos = vị trí bắt đầu (0-based) các lần khớp
// Bẫy: pi[i] là biên THẬT SỰ (proper) dài nhất của s[0..i]; mẫu rỗng trả rỗng; phân biệt hoa/thường.
// CSES: 1732 1733 1753 2107
// Ý nghĩa: tính hàm tiền tố (prefix function) của xâu: pi[i] = độ dài biên (vừa là tiền tố vừa là hậu tố, thật sự) dài nhất của s[0..i].
// Tham số: s = xâu đầu vào.
// Trả về: mảng pi cùng độ dài với s.
vector<int> prefixFunction(const string& s) {
    int n = (int)s.size();
    vector<int> pi(n, 0);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}
// Ý nghĩa: tìm mọi lần xuất hiện của mẫu p trong văn bản t bằng KMP.
// Tham số: t = văn bản; p = mẫu cần tìm.
// Trả về: danh sách vị trí bắt đầu (0-based) mỗi lần p khớp trong t (rỗng nếu p rỗng hoặc không khớp).
vector<int> kmpSearch(const string& t, const string& p) {
    vector<int> res;
    int m = (int)p.size();
    if (m == 0) return res;
    vector<int> pi = prefixFunction(p);
    int j = 0;
    for (int i = 0; i < (int)t.size(); i++) {
        while (j && t[i] != p[j]) j = pi[j - 1];
        if (t[i] == p[j]) j++;
        if (j == m) { res.push_back(i - m + 1); j = pi[j - 1]; }
    }
    return res;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ kmp.hpp -o demo && ./demo
int main() {
    auto occ = kmpSearch("ababab", "ab");     // 0, 2, 4
    auto pi = prefixFunction("aabaa");         // [0,1,0,1,2]
    printf("kmp: số lần khớp = %d (vị trí đầu %d) ; biên dài nhất 'aabaa' = %d\n",
           (int)occ.size(), occ[0], pi.back());   // 3 (0) ; 2
    return 0;
}
#endif
