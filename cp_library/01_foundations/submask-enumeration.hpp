#pragma once
#include <bits/stdc++.h>
using namespace std;
// Submask Enumeration — duyệt mọi mặt nạ con của mask (SOS DP, bao hàm-loại trừ).
// Khi dùng: DP trên tập con, chia mask thành cặp bù (s, mask ^ s).
// ĐPT: tổng trên mọi mask 0..2^n-1 là O(3^n); một mask là O(2^popcount(mask)).
// Dùng (idiom): for (int s = m; ; s = (s - 1) & m) { /* xử lý s */ if (s == 0) break; }
//        Helper: allSubmasks(m) trả vector mọi submask (giảm dần, gồm cả 0).
// Bẫy: kiểm s == 0 SAU khi xử lý rồi mới break; (s-1)&m mới nhảy đúng submask kế.
// CSES: 3308
inline vector<int> allSubmasks(int m) {
    vector<int> res;
    for (int s = m;; s = (s - 1) & m) {
        res.push_back(s);
        if (s == 0) break;
    }
    return res;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ submask-enumeration.hpp -o demo && ./demo
int main() {
    for (int s : allSubmasks(5)) printf("%d ", s);   // 5 4 1 0
    printf("\n");
    return 0;
}
#endif
