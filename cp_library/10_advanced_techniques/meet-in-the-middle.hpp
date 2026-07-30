#pragma once
#include <bits/stdc++.h>
using namespace std;
// Meet in the Middle — chia n phần tử làm hai nửa, liệt kê 2^(n/2) tổng mỗi nửa rồi ghép, O(2^(n/2) * n).
// Khi dùng: n <= ~40 (2^n quá lớn nhưng 2^(n/2) ổn): đếm/tìm tập con theo tổng, bài "tổng bằng S".
// ĐPT: O(2^(n/2) * (n + log)); bộ nhớ O(2^(n/2)). Nửa trái enum, nửa phải sort + tra cứu nhị phân.
// Dùng: long long c = countSubsetsWithSum(a, S); // số tập con (kể cả rỗng) có tổng đúng bằng S
// Bẫy: n <= ~40; đếm CẢ tập rỗng (tổng 0); dùng long long cho tổng; giá trị có thể âm.
// CSES: 1623 1628 1642 3425
// Ý nghĩa: đếm số tập con của a có tổng đúng bằng S bằng kỹ thuật gặp nhau ở giữa.
// Tham số: a = mảng số (n <= ~40); S = tổng mục tiêu.
// Trả về: số tập con (kể cả tập rỗng) có tổng = S.
long long countSubsetsWithSum(const vector<long long>& a, long long S) {
    int n = (int)a.size(), half = n / 2, rn = n - half;
    vector<long long> left, right;
    left.reserve(1 << half); right.reserve(1 << rn);
    for (int mask = 0; mask < (1 << half); mask++) {
        long long s = 0;
        for (int i = 0; i < half; i++) if (mask >> i & 1) s += a[i];
        left.push_back(s);
    }
    for (int mask = 0; mask < (1 << rn); mask++) {
        long long s = 0;
        for (int i = 0; i < rn; i++) if (mask >> i & 1) s += a[half + i];
        right.push_back(s);
    }
    sort(right.begin(), right.end());
    long long cnt = 0;
    for (long long l : left) {
        long long need = S - l;
        cnt += upper_bound(right.begin(), right.end(), need) - lower_bound(right.begin(), right.end(), need);
    }
    return cnt;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ meet-in-the-middle.hpp -o demo && ./demo
int main() {
    vector<long long> a = {1, 2, 3, 4, 5};
    printf("số tập con có tổng = 5: %lld\n", countSubsetsWithSum(a, 5));   // {5},{1,4},{2,3} -> 3
    return 0;
}
#endif
