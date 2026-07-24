#pragma once
#include <bits/stdc++.h>
using namespace std;
// Sliding Window — cửa sổ [l, r] co giãn để giữ điều kiện; đếm/tối ưu O(n).
// Khi dùng: tính hợp lệ ĐƠN ĐIỆU theo độ dài (thêm phần tử làm xấu đi, bớt làm tốt lên).
// ĐPT: O(n) — mỗi phần tử vào và ra cửa sổ đúng một lần.
// Dùng (mẫu longest): for (r) { add(a[r]); while(!ok()) { remove(a[l]); l++; } ans=max(ans, r-l+1); }
//        Helper sẵn: countSubarraysSumAtMost(a, K) — số đoạn con có tổng <= K (a >= 0).
// Bẫy: chỉ đúng khi điều kiện đơn điệu theo độ dài; countSubarrays cần phần tử KHÔNG âm.
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
