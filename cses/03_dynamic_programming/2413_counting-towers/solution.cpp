#include <iostream>
#include <unordered_map>

using namespace std;

constexpr long long MODULO = 1'000'000'007;

// Ma trận 2x2 dùng cho lũy thừa ma trận của công thức truy hồi (A, B).
struct Matrix {
    long long topLeft;
    long long topRight;
    long long bottomLeft;
    long long bottomRight;
};

// Nhân hai ma trận 2x2 theo modulo.
Matrix multiply(const Matrix& first, const Matrix& second) {
    return {
        (first.topLeft * second.topLeft +
         first.topRight * second.bottomLeft) % MODULO,
        (first.topLeft * second.topRight +
         first.topRight * second.bottomRight) % MODULO,
        (first.bottomLeft * second.topLeft +
         first.bottomRight * second.bottomLeft) % MODULO,
        (first.bottomLeft * second.topRight +
         first.bottomRight * second.bottomRight) % MODULO
    };
}

// Lũy thừa ma trận bằng bình phương liên tiếp (fast exponentiation) trong O(log n).
Matrix matrixPower(Matrix base, int exponent) {
    Matrix result{1, 0, 0, 1};  // Khởi tạo là ma trận đơn vị.
    while (exponent > 0) {
        if (exponent & 1) {
            result = multiply(result, base);
        }
        base = multiply(base, base);
        exponent >>= 1;
    }
    return result;
}

// answer(n) = (A[n] + B[n]) mod p, với [A[n], B[n]]^T = M^(n-1) * [1, 1]^T.
long long countTowers(int height) {
    // M = [[2, 1], [1, 4]]; nhân với vector [1, 1]^T là cộng hai cột lại.
    const Matrix power = matrixPower({2, 1, 1, 4}, height - 1);
    const long long connected = (power.topLeft + power.topRight) % MODULO;
    const long long split = (power.bottomLeft + power.bottomRight) % MODULO;
    return (connected + split) % MODULO;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int tests;
    cin >> tests;

    // Bộ nhớ đệm theo chiều cao để không tính lại các truy vấn trùng nhau.
    unordered_map<int, long long> cache;
    cache.reserve(tests * 2);
    while (tests--) {
        int height;
        cin >> height;
        const auto [iterator, inserted] = cache.emplace(height, 0);
        if (inserted) {
            iterator->second = countTowers(height);
        }
        cout << iterator->second << '\n';
    }
    return 0;
}
