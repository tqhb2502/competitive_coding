#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr long long MOD = 1'000'000'007LL;
constexpr int MAX_A = 1'000'000;

// Lũy thừa modulo (binary exponentiation) để tính nghịch đảo theo Fermat
long long mod_power(long long base, long long exponent) {
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

    // Tiền xử lý giai thừa: factorial[i] = i! mod p
    vector<long long> factorial(MAX_A + 1, 1);
    vector<long long> inverse_factorial(MAX_A + 1, 1);
    for (size_t i = 1; i <= static_cast<size_t>(MAX_A); ++i) {
        factorial[i] = factorial[i - 1] * static_cast<long long>(i) % MOD;
    }

    // Chỉ một lần pow (Fermat) cho inverse factorial lớn nhất, rồi truy hồi ngược:
    // inverse_factorial[i-1] = inverse_factorial[i] * i mod p
    inverse_factorial[MAX_A] = mod_power(factorial[MAX_A], MOD - 2);
    for (size_t i = static_cast<size_t>(MAX_A); i >= 1; --i) {
        inverse_factorial[i - 1] =
            inverse_factorial[i] * static_cast<long long>(i) % MOD;
    }

    int queries;
    cin >> queries;
    while (queries--) {
        // Mỗi truy vấn (a, b): C(a, b) = a! * inv(b!) * inv((a-b)!) mod p
        // Đề bảo đảm 0 <= b <= a nên không cần xử lý trường hợp b > a
        int a, b;
        cin >> a >> b;
        const size_t a_index = static_cast<size_t>(a);
        const size_t b_index = static_cast<size_t>(b);
        long long answer = factorial[a_index] * inverse_factorial[b_index] % MOD;
        answer = answer * inverse_factorial[a_index - b_index] % MOD;
        cout << answer << '\n';
    }
}
