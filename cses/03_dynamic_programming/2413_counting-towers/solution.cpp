#include <iostream>
#include <unordered_map>

using namespace std;

constexpr long long MODULO = 1'000'000'007;

struct Matrix {
    long long topLeft;
    long long topRight;
    long long bottomLeft;
    long long bottomRight;
};

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

Matrix matrixPower(Matrix base, int exponent) {
    Matrix result{1, 0, 0, 1};
    while (exponent > 0) {
        if (exponent & 1) {
            result = multiply(result, base);
        }
        base = multiply(base, base);
        exponent >>= 1;
    }
    return result;
}

long long countTowers(int height) {
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
