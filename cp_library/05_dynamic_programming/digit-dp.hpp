#pragma once
#include <bits/stdc++.h>
using namespace std;
// Digit DP — đếm số nguyên trong [0, N] thỏa tính chất trên các CHỮ SỐ (mẫu: không có hai chữ số kề nhau bằng nhau).
// Khi dùng: đếm số trong đoạn theo ràng buộc chữ số (kề nhau, tổng chữ số, chứa mẫu...); ghép f(b)-f(a-1) cho [a, b].
// ĐPT: O(số_chữ_số * trạng_thái * 10); bộ nhớ theo số trạng thái. Trạng thái: (vị trí, chữ số trước, đang-tight, đã-bắt-đầu).
// Dùng: long long x = countNoAdjEqual(N); // đếm [0, N]; đoạn [a,b] = countNoAdjEqual(b) - countNoAdjEqual(a-1)
// Bẫy: xử lý số 0 đứng đầu bằng cờ "started"; chỉ nhớ hóa khi KHÔNG tight; đổi điều kiện trong vòng để giải bài khác.
// CSES: 2174 2220
// Ý nghĩa: đếm số nguyên trong [0, N] mà không có hai chữ số liền kề bằng nhau (mẫu digit DP).
// Tham số: N = cận trên (bao gồm); nếu N < 0 trả 0.
// Trả về: số lượng số nguyên hợp lệ trong [0, N].
long long countNoAdjEqual(long long N) {
    if (N < 0) return 0;
    string s = to_string(N);
    int L = (int)s.size();
    // memo[pos][prev][started] cho nhánh KHÔNG tight; prev = 10 nghĩa là chưa có chữ số nào.
    vector<vector<array<long long, 2>>> memo(L, vector<array<long long, 2>>(11, {-1, -1}));
    function<long long(int, int, bool, bool)> go = [&](int pos, int prev, bool tight, bool started) -> long long {
        if (pos == L) return 1;   // đã xếp xong một số hợp lệ (kể cả số 0)
        if (!tight && memo[pos][prev][started] != -1) return memo[pos][prev][started];
        int hi = tight ? s[pos] - '0' : 9;
        long long res = 0;
        for (int d = 0; d <= hi; d++) {
            if (started && d == prev) continue;   // cấm hai chữ số kề nhau bằng nhau
            bool nstarted = started || d > 0;
            int nprev = nstarted ? d : 10;
            res += go(pos + 1, nprev, tight && d == hi, nstarted);
        }
        if (!tight) memo[pos][prev][started] = res;
        return res;
    };
    return go(0, 10, true, false);
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ digit-dp.hpp -o demo && ./demo
int main() {
    // [0, 11]: hợp lệ là 0..10 (11 số); 11 bị loại vì '1','1' kề nhau bằng nhau.
    printf("đếm [0,11] không có chữ số kề bằng nhau = %lld\n", countNoAdjEqual(11));   // 11
    return 0;
}
#endif
