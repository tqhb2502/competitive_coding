#include <iostream>

using namespace std;

constexpr long long MOD = 1'000'000'007;

// Lũy thừa nhanh (binary exponentiation): tính base^exponent mod MOD trong O(log exponent)
long long modular_power(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        // Nếu bit thấp nhất của số mũ bằng 1 thì nhân base hiện tại vào kết quả
        if (exponent % 2 == 1) {
            result = result * base % MOD;
        }
        // Bình phương base để chuyển sang bit tiếp theo của số mũ
        base = base * base % MOD;
        exponent /= 2;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    // Số chuỗi bit độ dài n bằng 2^n mod (10^9 + 7)
    cout << modular_power(2, n) << '\n';
    return 0;
}
