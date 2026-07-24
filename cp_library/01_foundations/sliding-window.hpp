#pragma once
#include <bits/stdc++.h>
using namespace std;
// Sliding Window — cửa sổ [l, r] co giãn để giữ điều kiện; đếm/tối ưu O(n).
// Khi dùng: tính hợp lệ ĐƠN ĐIỆU theo độ dài (thêm phần tử làm xấu đi, bớt làm tốt lên).
// ĐPT: O(n) — mỗi phần tử vào và ra cửa sổ đúng một lần.
// Dùng (mẫu longest): for (r) { add(a[r]); while(!ok()) { remove(a[l]); l++; } ans=max(ans, r-l+1); }
//        Helper sẵn: countSubarraysSumAtMost(a, K) — số đoạn con có tổng <= K (a >= 0).
// Bẫy: chỉ đúng khi điều kiện đơn điệu theo độ dài; countSubarrays cần phần tử KHÔNG âm.
// CSES: 1076 1077 1141 1159 1191 1633 1644 1660 2229 2428 3190 3219 3220 3221 3222 3223 3224 3405 3416 3420 3426
inline long long countSubarraysSumAtMost(const vector<long long>& a, long long K) {
    long long c = 0, sum = 0;
    int l = 0;
    for (int r = 0; r < (int)a.size(); r++) {
        sum += a[r];
        while (l <= r && sum > K) { sum -= a[l]; l++; }
        c += r - l + 1;  // số đoạn kết thúc tại r và còn hợp lệ
    }
    return c;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ sliding-window.hpp -o demo && ./demo
int main() {
    vector<long long> a = {1, 2, 3};
    printf("so doan con co tong<=3: %lld\n", countSubarraysSumAtMost(a, 3));   // 4
    return 0;
}
#endif
