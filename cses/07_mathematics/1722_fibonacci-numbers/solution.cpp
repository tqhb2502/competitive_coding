#include <iostream>
#include <utility>

using namespace std;

namespace {
constexpr long long MOD = 1'000'000'007LL;

// Fast doubling: trả về cặp (F(n), F(n+1)) mod MOD.
// Đệ quy chia đôi n, dùng hai đẳng thức
//   F(2k)   = F(k) * (2*F(k+1) - F(k))
//   F(2k+1) = F(k)^2 + F(k+1)^2
pair<long long, long long> fibonacci_pair(unsigned long long n) {
    if (n == 0) {
        return {0, 1};
    }

    // a = F(k), b = F(k+1) với k = n / 2
    const auto [a, b] = fibonacci_pair(n >> 1ULL);
    // Cộng thêm MOD trước khi lấy mod vì (2*F(k+1) - F(k)) có thể âm
    const long long even = a * ((2 * b % MOD - a + MOD) % MOD) % MOD;  // F(2k)
    const long long odd = (a * a % MOD + b * b % MOD) % MOD;           // F(2k+1)

    // Nếu bit hiện tại là 1 thì dịch thêm một bước Fibonacci
    if (n & 1ULL) {
        return {odd, (even + odd) % MOD};
    }
    return {even, odd};
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unsigned long long n;
    cin >> n;
    cout << fibonacci_pair(n).first << '\n';
    return 0;
}
