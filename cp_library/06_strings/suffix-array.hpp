#pragma once
#include <bits/stdc++.h>
using namespace std;
// Suffix Array — mảng chỉ số hậu tố đã SẮP theo thứ tự từ điển + mảng LCP (Kasai), O(n log^2 n) / O(n).
// Khi dùng: đếm xâu con phân biệt, tìm xâu con lặp dài nhất, so khớp mẫu, thứ tự từ điển các hậu tố.
// ĐPT: suffixArray O(n log^2 n), lcpArray O(n); bộ nhớ O(n). sa[k] = vị trí bắt đầu hậu tố hạng k (0-based).
// Dùng: auto sa = suffixArray(s); auto lcp = lcpArray(s, sa); // lcp[i] = LCP(hậu tố sa[i], sa[i+1]), cỡ n-1
// Bẫy: không thêm ký tự canh gác (dùng rank = -1 khi vượt biên); số xâu con phân biệt = n*(n+1)/2 - tổng lcp; xâu rỗng -> rỗng.
// CSES: 2105 2106 3225
// Ý nghĩa: dựng mảng hậu tố (suffix array) của xâu bằng sắp xếp nhân đôi độ dài (prefix doubling).
// Tham số: s = xâu đầu vào.
// Trả về: mảng sa với sa[k] = chỉ số bắt đầu của hậu tố đứng hạng k theo thứ tự từ điển.
vector<int> suffixArray(const string& s) {
    int n = (int)s.size();
    if (n == 0) return {};
    vector<int> sa(n), rnk(n), tmp(n);
    for (int i = 0; i < n; i++) { sa[i] = i; rnk[i] = (unsigned char)s[i]; }
    for (int k = 1;; k <<= 1) {
        auto cmp = [&](int a, int b) {
            if (rnk[a] != rnk[b]) return rnk[a] < rnk[b];
            int ra = a + k < n ? rnk[a + k] : -1, rb = b + k < n ? rnk[b + k] : -1;
            return ra < rb;
        };
        sort(sa.begin(), sa.end(), cmp);
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; i++) tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
        for (int i = 0; i < n; i++) rnk[i] = tmp[i];
        if (rnk[sa[n - 1]] == n - 1 || k >= n) break;
    }
    return sa;
}
// Ý nghĩa: dựng mảng LCP giữa các hậu tố kề nhau trong suffix array bằng thuật toán Kasai.
// Tham số: s = xâu; sa = suffix array của s.
// Trả về: mảng lcp cỡ n-1 với lcp[i] = độ dài tiền tố chung dài nhất của hậu tố sa[i] và sa[i+1].
vector<int> lcpArray(const string& s, const vector<int>& sa) {
    int n = (int)s.size();
    vector<int> rnk(n), lcp(n > 0 ? n - 1 : 0, 0);
    for (int i = 0; i < n; i++) rnk[sa[i]] = i;
    int h = 0;
    for (int i = 0; i < n; i++) {
        if (rnk[i] > 0) {
            int j = sa[rnk[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
            lcp[rnk[i] - 1] = h;
            if (h) h--;
        } else h = 0;
    }
    return lcp;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ suffix-array.hpp -o demo && ./demo
int main() {
    string s = "banana";
    auto sa = suffixArray(s);
    auto lcp = lcpArray(s, sa);
    printf("suffix array:");
    for (int x : sa) printf(" %d", x);     // 5 3 1 0 4 2
    printf("\nlcp:");
    for (int x : lcp) printf(" %d", x);     // 1 3 0 0 2
    printf("\n");
    return 0;
}
#endif
