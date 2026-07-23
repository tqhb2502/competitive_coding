#include <iostream>

using namespace std;

namespace {
// modular exponentiation: tính base^exponent mod modulus trong O(log exponent).
long long mod_power(long long base, long long exponent, long long modulus) {
    long long result = 1;
    base %= modulus;
    while (exponent > 0) {
        if (exponent & 1LL) {
            result = result * base % modulus;
        }
        base = base * base % modulus;
        exponent >>= 1LL;
    }
    return result;
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr long long MOD = 1'000'000'007LL;  // số nguyên tố p
    constexpr long long PHI = MOD - 1;          // phi(p) = p - 1 vì p nguyên tố

    int queries;
    cin >> queries;
    while (queries--) {
        long long a, b, c;
        cin >> a >> b >> c;

        // Trường hợp a = 0: không dùng Fermat được vì gcd(0, p) != 1.
        // Số mũ thật e = b^c bằng 0 khi và chỉ khi b = 0 và c > 0 -> 0^0 = 1;
        // ngược lại e > 0 -> 0^(số dương) = 0.
        if (a == 0) {
            const bool exponent_is_zero = (b == 0 && c > 0);
            cout << (exponent_is_zero ? 1 : 0) << '\n';
            continue;
        }

        // a > 0 nên gcd(a, p) = 1: rút gọn số mũ theo mod PHI rồi lũy thừa theo mod p.
        const long long reduced_exponent = mod_power(b, c, PHI);
        cout << mod_power(a, reduced_exponent, MOD) << '\n';
    }
}
