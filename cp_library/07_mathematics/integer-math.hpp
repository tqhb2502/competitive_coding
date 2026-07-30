#pragma once
#include <bits/stdc++.h>
using namespace std;
// Integer Math — căn nguyên chính xác, valuation của n!, LCM có chặn và truy vấn chữ số trong chuỗi số.
// Khi dùng: tránh sai số sqrt; trailing zeros/factorial prime exponent; chu kỳ gcd/lcm; nhóm số theo độ dài.
// ĐPT: sqrt O(log 2^64), factorial exponent/digit query O(log n), lcm O(log min); bộ nhớ O(1).
// Dùng: floorSqrtU64(n); factorialPrimeExponent(n,p); lcmCapped(a,b,cap); digitInPositiveIntegers(k);
// Bẫy: k của digitInPositiveIntegers là 1-based; valuation cần p nguyên tố; lcmCapped trả cap+1 nếu vượt.
// CSES: 1071 1072 1083 1618 1754 2431 3214 3215 3216 3218 3398 3406
// Ý nghĩa: căn bậc hai lấy sàn chính xác của số uint64_t.
// Tham số: value = số cần lấy căn.
// Trả về: số lớn nhất root sao cho root*root<=value.
uint64_t floorSqrtU64(uint64_t value) {
    uint64_t low = 0, high = uint64_t(1) << 32;
    while (low + 1 < high) {
        uint64_t middle = low + (high - low) / 2;
        if ((unsigned __int128)middle * middle <= value) low = middle;
        else high = middle;
    }
    return low;
}

// Ý nghĩa: kiểm tra số uint64_t có phải chính phương hay không.
// Tham số: value = số cần kiểm tra.
// Trả về: true nếu tồn tại root nguyên với root²=value.
bool isPerfectSquareU64(uint64_t value) {
    uint64_t root = floorSqrtU64(value);
    return root * root == value;
}

// Ý nghĩa: tính số mũ của prime trong n! theo công thức Legendre.
// Tham số: n = giai thừa; prime = số nguyên tố >=2.
// Trả về: tổng floor(n/prime^i).
uint64_t factorialPrimeExponent(uint64_t n, uint64_t prime) {
    if (prime < 2) throw invalid_argument("factorialPrimeExponent: prime < 2");
    uint64_t answer = 0;
    while (n) {
        n /= prime;
        answer += n;
    }
    return answer;
}

// Ý nghĩa: tính lcm(a,b) nhưng bão hòa ở cap+1.
// Tham số: a,b = số không âm; cap < UINT64_MAX.
// Trả về: lcm nếu <=cap; cap+1 nếu vượt; lcm(0,b)=0.
uint64_t lcmCapped(uint64_t a, uint64_t b, uint64_t cap) {
    if (cap == UINT64_MAX) throw invalid_argument("lcmCapped: cap phải < UINT64_MAX");
    if (a == 0 || b == 0) return 0;
    unsigned __int128 value = (unsigned __int128)(a / std::gcd(a, b)) * b;
    return value > cap ? cap + 1 : (uint64_t)value;
}

// Ý nghĩa: lấy chữ số thứ k trong xâu vô hạn "123456789101112..." (k 1-based).
// Tham số: k = vị trí dương.
// Trả về: chữ số 0..9.
int digitInPositiveIntegers(uint64_t k) {
    if (k == 0) throw invalid_argument("digitInPositiveIntegers: k phải 1-based");
    uint64_t digits = 1, start = 1;
    while (digits < 19) {
        unsigned __int128 block = (unsigned __int128)9 * start * digits;
        if ((unsigned __int128)k <= block) break;
        k -= (uint64_t)block;
        ++digits;
        start *= 10;
    }
    uint64_t number = start + (k - 1) / digits;
    int index = (int)((k - 1) % digits);
    for (uint64_t skip = digits - 1; skip > (uint64_t)index; --skip) number /= 10;
    return (int)(number % 10);
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ integer-math.hpp -o demo && ./demo
int main() {
    printf("sqrt(99)=%llu, v5(100!)=%llu, digit#15=%d, lcm=%llu\n",
           (unsigned long long)floorSqrtU64(99),
           (unsigned long long)factorialPrimeExponent(100, 5),
           digitInPositiveIntegers(15),
           (unsigned long long)lcmCapped(12, 18, 100));  // 9, 24, 2, 36
    return 0;
}
#endif
