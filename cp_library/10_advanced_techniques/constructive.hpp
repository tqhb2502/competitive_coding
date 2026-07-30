#pragma once
#include <bits/stdc++.h>
using namespace std;
// Constructive Algorithms — các khối dựng hoán vị: chẵn-lẻ, dịch vòng và hoán vị có đúng k nghịch thế.
// Khi dùng: đề chấp nhận bất kỳ cấu hình hợp lệ; cần xây trực tiếp rồi chứng minh invariant thay vì tối ưu/tìm kiếm.
// ĐPT: các hàm O(n); bộ nhớ O(n).
// Dùng: evenOddBeautifulPermutation(n); cyclicShiftPermutation(n,k); permutationWithInversions(n,k);
// Bẫy: mọi output ở đây là hoán vị 0-based trừ evenOdd trả 1-based; k nghịch thế phải trong [0,n(n-1)/2].
// CSES: 1070 1092 1697 1698 1755 2165 2214 2215 2418 2423 2430 3140 3175 3311 3399 3419 3422 3423 3424
// Ý nghĩa: dựng Beautiful Permutation CSES bằng cách in số chẵn rồi số lẻ (giá trị 1-based).
// Tham số: n = kích thước.
// Trả về: hoán vị hợp lệ; rỗng nếu n=2 hoặc n=3.
vector<int> evenOddBeautifulPermutation(int n) {
    if (n < 0) throw invalid_argument("evenOddBeautifulPermutation: n âm");
    if (n == 2 || n == 3) return {};
    vector<int> answer;
    for (int x = 2; x <= n; x += 2) answer.push_back(x);
    for (int x = 1; x <= n; x += 2) answer.push_back(x);
    return answer;
}

// Ý nghĩa: dựng hoán vị 0-based p[i]=(i+shift) mod n.
// Tham số: n = kích thước; shift = độ dịch bất kỳ (âm được chuẩn hóa).
// Trả về: hoán vị dịch vòng.
vector<int> cyclicShiftPermutation(int n, long long shift) {
    if (n < 0) throw invalid_argument("cyclicShiftPermutation: n âm");
    if (n == 0) return {};
    shift %= n;
    if (shift < 0) shift += n;
    vector<int> answer(n);
    for (int i = 0; i < n; ++i) answer[i] = (i + (int)shift) % n;
    return answer;
}

// Ý nghĩa: dựng hoán vị 0-based độ dài n có đúng k nghịch thế bằng cách lấy hai đầu dải giá trị.
// Tham số: n = kích thước; k = số nghịch thế yêu cầu.
// Trả về: hoán vị; rỗng nếu k ngoài [0,n(n-1)/2].
vector<int> permutationWithInversions(int n, long long k) {
    if (n < 0) throw invalid_argument("permutationWithInversions: n âm");
    long long maximum = 1LL * n * (n - 1) / 2;
    if (k < 0 || k > maximum) return {};
    vector<int> answer;
    answer.reserve(n);
    int low = 0, high = n - 1;
    while (low <= high) {
        long long gain = high - low;
        if (k >= gain) {
            answer.push_back(high--);
            k -= gain;
        } else {
            // Phần còn lại tăng dần có 0 inversion; đưa phần tử low+k lên đầu tạo đúng k.
            int chosen = low + (int)k;
            answer.push_back(chosen);
            for (int x = low; x < chosen; ++x) answer.push_back(x);
            for (int x = chosen + 1; x <= high; ++x) answer.push_back(x);
            break;
        }
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ constructive.hpp -o demo && ./demo
int main() {
    auto p = permutationWithInversions(5, 7);
    long long inversions = 0;
    for (int i = 0; i < 5; ++i) for (int j = i + 1; j < 5; ++j) inversions += p[i] > p[j];
    printf("beautiful-size=%zu; inversion=%lld; shift-first=%d\n",
           evenOddBeautifulPermutation(5).size(), inversions, cyclicShiftPermutation(5, 2)[0]);
    return 0;
}
#endif
