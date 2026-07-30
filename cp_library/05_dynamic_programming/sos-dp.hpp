#pragma once
#include <bits/stdc++.h>
using namespace std;
// SOS DP (Sum over Subsets) — với mọi mask, tính tổng f trên MỌI tập con (hoặc tập cha) của mask, O(n * 2^n).
// Khi dùng: cần tổng/tích hợp theo quan hệ tập con-tập cha trên toàn bộ 2^n mask (đếm cặp AND/OR, tích chập tập con).
// ĐPT: O(n * 2^n) (nhanh hơn O(3^n) duyệt trực tiếp submask); bộ nhớ O(2^n). f có kích thước ĐÚNG 2^n.
// Dùng: auto F = sosSubset(f); // F[mask] = tổng f[sub] với sub là tập con của mask; sosSuperset cho tập cha
// Bẫy: f.size() phải là lũy thừa 2 (= 2^n); dùng long long tránh tràn; "con" gồm chính mask và tập rỗng.
// CSES: 1654 3141 3194 3195
// Ý nghĩa: với mỗi mask, tính tổng f trên mọi tập CON của mask (biến đổi zeta theo chiều tập con).
// Tham số: f = mảng kích thước 2^n (f[mask] = giá trị tại mask).
// Trả về: mảng F cùng cỡ với F[mask] = tổng f[sub] trên mọi sub là tập con của mask.
vector<long long> sosSubset(vector<long long> f) {
    int N = (int)f.size();
    for (int i = 0; (1 << i) < N; i++)
        for (int mask = 0; mask < N; mask++)
            if (mask >> i & 1) f[mask] += f[mask ^ (1 << i)];
    return f;
}
// Ý nghĩa: với mỗi mask, tính tổng f trên mọi tập CHA của mask (biến đổi zeta theo chiều tập cha / superset).
// Tham số: f = mảng kích thước 2^n.
// Trả về: mảng F cùng cỡ với F[mask] = tổng f[sup] trên mọi sup chứa mask.
vector<long long> sosSuperset(vector<long long> f) {
    int N = (int)f.size();
    for (int i = 0; (1 << i) < N; i++)
        for (int mask = 0; mask < N; mask++)
            if (!(mask >> i & 1)) f[mask] += f[mask | (1 << i)];
    return f;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ sos-dp.hpp -o demo && ./demo
int main() {
    vector<long long> f = {1, 2, 3, 4};   // n=2: mask 00,01,10,11
    auto F = sosSubset(f);
    printf("tổng trên mọi tập con của 11 = %lld ; của 01 = %lld\n", F[3], F[1]);   // 1+2+3+4=10 ; 1+2=3
    return 0;
}
#endif
