#pragma once
#include <bits/stdc++.h>
using namespace std;
// Z-function — z[i] = độ dài tiền tố chung dài nhất của s và hậu tố s[i..], O(n); dùng để khớp mẫu, tìm chu kỳ.
// Khi dùng: khớp mẫu (nối p + sep + t), tìm biên/chu kỳ, so tiền tố nhanh; thay thế KMP trong nhiều bài.
// ĐPT: zFunction O(n), zSearch O(|p| + |t|); bộ nhớ O(n). Quy ước z[0] = n (toàn bộ xâu).
// Dùng: auto z = zFunction(s); auto pos = zSearch(t, p); // pos = vị trí bắt đầu (0-based) các lần khớp
// Bẫy: z[0] đặt = n; zSearch dùng ký tự ngăn cách '\\x01' (phải KHÔNG xuất hiện trong t, p); mẫu rỗng trả rỗng.
// CSES: 1753 2107
// Ý nghĩa: tính mảng Z của xâu: z[i] = độ dài tiền tố chung dài nhất giữa s và s[i..] (z[0] = |s|).
// Tham số: s = xâu đầu vào.
// Trả về: mảng z cùng độ dài với s.
vector<int> zFunction(const string& s) {
    int n = (int)s.size();
    vector<int> z(n, 0);
    if (n) z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i < r) z[i] = min(r - i, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] > r) { l = i; r = i + z[i]; }
    }
    return z;
}
// Ý nghĩa: tìm mọi lần xuất hiện của mẫu p trong văn bản t bằng Z-function trên xâu nối p + sep + t.
// Tham số: t = văn bản; p = mẫu.
// Trả về: danh sách vị trí bắt đầu (0-based) mỗi lần p khớp trong t (rỗng nếu p rỗng).
vector<int> zSearch(const string& t, const string& p) {
    vector<int> res;
    int m = (int)p.size();
    if (m == 0) return res;
    string s = p + '\x01' + t;
    vector<int> z = zFunction(s);
    for (int i = m + 1; i < (int)s.size(); i++) if (z[i] >= m) res.push_back(i - m - 1);
    return res;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ z-function.hpp -o demo && ./demo
int main() {
    auto z = zFunction("aabaa");        // [5,1,0,2,1]
    auto occ = zSearch("ababab", "ab"); // 0, 2, 4
    printf("z: số lần khớp = %d ; z[3] của 'aabaa' = %d\n", (int)occ.size(), z[3]);   // 3 ; 2
    return 0;
}
#endif
