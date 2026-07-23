#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace {
constexpr int64_t MOD = 1'000'000'007;

// Lũy thừa nhị phân theo modulo (modpow): base^exponent mod MOD
int64_t mod_power(int64_t base, int exponent) {
    int64_t result = 1;
    while (exponent > 0) {
        if (exponent & 1) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent >>= 1;
    }
    return result;
}
}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int length;       // n: độ dài dãy
    int value_count;  // k: số giá trị trong bảng chữ cái
    cin >> length >> value_count;

    // Tiền xử lý factorial và inverse factorial để tính C(k, i) trong O(1)
    vector<int64_t> factorial(value_count + 1, 1);
    vector<int64_t> inverse_factorial(value_count + 1, 1);
    for (int value = 1; value <= value_count; ++value) {
        factorial[value] = factorial[value - 1] * value % MOD;
    }
    // Nghịch đảo modulo bằng định lý Fermat: x^(-1) = x^(p-2) mod p
    inverse_factorial[value_count] =
        mod_power(factorial[value_count], static_cast<int>(MOD - 2));
    for (int value = value_count; value >= 1; --value) {
        inverse_factorial[value - 1] = inverse_factorial[value] * value % MOD;
    }

    // Nguyên lý bù trừ: answer = sum (-1)^i * C(k, i) * (k - i)^n
    int64_t answer = 0;
    for (int omitted = 0; omitted <= value_count; ++omitted) {
        // Số cách chọn omitted giá trị bị cấm: C(k, omitted)
        int64_t combinations = factorial[value_count] * inverse_factorial[omitted] % MOD;
        combinations = combinations * inverse_factorial[value_count - omitted] % MOD;
        // Số dãy dùng đúng phần còn lại: (k - omitted)^n
        const int64_t term =
            combinations * mod_power(value_count - omitted, length) % MOD;

        if (omitted % 2 == 0) {
            // omitted chẵn: cộng vào kết quả
            answer += term;
            if (answer >= MOD) {
                answer -= MOD;
            }
        } else {
            // omitted lẻ: trừ khỏi kết quả
            answer -= term;
            if (answer < 0) {
                answer += MOD;
            }
        }
    }

    cout << answer << '\n';
}
