#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace {
constexpr long long MOD = 1'000'000'007LL;

// Lũy thừa modulo nhanh: tính base^exponent mod MOD.
long long mod_pow(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        if (exponent & 1LL) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1LL;
    }
    return result;
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    // Tính trước giai thừa fact[i] = i! mod MOD.
    const size_t n = s.size();
    vector<long long> factorial(n + 1, 1);
    for (size_t i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * static_cast<long long>(i) % MOD;
    }

    // Đếm số lần xuất hiện của từng ký tự a-z.
    array<int, 26> count{};
    for (char character : s) {
        ++count[static_cast<size_t>(character - 'a')];
    }

    // Đáp án = n! / product(cnt[c]!) theo hệ số multinomial.
    // Phép chia thực hiện bằng modular inverse qua Fermat's little theorem.
    long long answer = factorial[n];
    for (int frequency : count) {
        answer = answer * mod_pow(factorial[static_cast<size_t>(frequency)], MOD - 2) % MOD;
    }

    cout << answer << '\n';
    return 0;
}
