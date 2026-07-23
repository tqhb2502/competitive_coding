#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr long long MOD = 1'000'000'007LL;
    constexpr long long INVERSE_TWO = 500'000'004LL;  // modular inverse của 2 mod 1e9+7

    long long n;
    cin >> n;

    // S = sum_{d=1..n} d * floor(n/d), duyệt theo từng khối divisor-block.
    long long answer = 0;
    long long left = 1;
    while (left <= n) {
        const long long quotient = n / left;           // thương floor(n/d) không đổi trên khối
        const long long right = n / quotient;           // chỉ số lớn nhất giữ nguyên thương
        // Tổng cấp số cộng left..right = (left + right) * số hạng / 2, lấy mod từng bước.
        const long long terms = (right - left + 1) % MOD;
        const long long endpoints = (left + right) % MOD;
        const long long interval_sum = endpoints * terms % MOD * INVERSE_TWO % MOD;
        // Nhân với thương q rồi cộng dồn vào kết quả.
        answer = (answer + quotient % MOD * interval_sum) % MOD;
        left = right + 1;
    }

    cout << answer << '\n';
}
