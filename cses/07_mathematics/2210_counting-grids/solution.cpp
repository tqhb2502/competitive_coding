#include <iostream>

constexpr long long MOD = 1'000'000'007LL;

// Lũy thừa modular (modular exponentiation) theo O(log exponent).
long long modular_power(long long base, long long exponent) {
    long long result = 1;
    while (exponent > 0) {
        if ((exponent & 1LL) != 0) {
            result = result * base % MOD;
        }
        base = base * base % MOD;
        exponent /= 2;
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long size;
    std::cin >> size;
    const long long cells = size * size;  // tổng số ô = n^2

    // Số orbit của mỗi phép xoay (bổ đề Burnside trên nhóm C4).
    // Xoay 90 độ và 270 độ đối xứng nên cho cùng số orbit.
    // n chẵn: orbit kích thước 4; n lẻ: thêm 1 orbit cho ô trung tâm.
    const long long quarter_turn_orbits =
        size % 2 == 0 ? cells / 4 : (cells + 3) / 4;
    // Xoay 180 độ: orbit kích thước 2; n lẻ thêm 1 orbit cho ô trung tâm.
    const long long half_turn_orbits =
        size % 2 == 0 ? cells / 2 : (cells + 1) / 2;

    // Fix(g) = 2^(số orbit của g): số cách tô bất biến dưới phép xoay g.
    const long long fixed_identity = modular_power(2, cells);      // xoay 0 độ
    const long long fixed_quarter = modular_power(2, quarter_turn_orbits);
    const long long fixed_half = modular_power(2, half_turn_orbits);

    // Kết quả Burnside = ( Fix(id) + 2*Fix(90) + Fix(180) ) / |G|, với |G| = 4.
    const long long total = (
        fixed_identity + 2 * fixed_quarter + fixed_half
    ) % MOD;
    // Chia cho 4 trên modulo bằng nhân với modular inverse của 4 (Fermat).
    const long long inverse_four = modular_power(4, MOD - 2);
    std::cout << total * inverse_four % MOD << '\n';
    return 0;
}
