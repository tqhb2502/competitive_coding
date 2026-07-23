#include <iostream>

using namespace std;

namespace {
constexpr long long MOD = 1'000'000'007LL;

// Binary exponentiation: tính base^exponent mod MOD trong O(log exponent).
// Tách exponent theo hệ nhị phân, nhân dồn base^(2^i) ứng với các bit 1.
long long mod_power(long long base, long long exponent) {
    long long result = 1;
    base %= MOD;
    while (exponent > 0) {
        // Bit thấp nhất bằng 1 thì góp base hiện tại vào kết quả.
        if (exponent & 1LL) {
            result = result * base % MOD;
        }
        // Bình phương base để chuyển sang bậc 2^(i+1), luôn lấy mod.
        base = base * base % MOD;
        exponent >>= 1LL;
    }
    return result;
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int queries;
    cin >> queries;
    // Mỗi truy vấn (a, b) in ra a^b mod (10^9 + 7); quy ước 0^0 = 1.
    while (queries--) {
        long long a, b;
        cin >> a >> b;
        cout << mod_power(a, b) << '\n';
    }
}
