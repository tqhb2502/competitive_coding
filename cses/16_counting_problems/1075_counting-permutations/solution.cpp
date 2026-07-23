#include <algorithm>
#include <iostream>

using namespace std;

constexpr int MOD = 1'000'000'007;
constexpr int MAX_N = 1000;

long long factorial[MAX_N + 1];        // giai thừa i!
long long inverseFactorial[MAX_N + 1]; // nghịch đảo giai thừa (i!)^{-1}
long long powerOfTwo[MAX_N + 1];       // lũy thừa 2^i theo modulo

// Lũy thừa nhanh theo modulo: base^exponent mod MOD
long long modPow(long long base, long long exponent) {
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    // Tiền xử lý giai thừa và lũy thừa của 2
    factorial[0] = 1;
    inverseFactorial[0] = 1;
    powerOfTwo[0] = 1;
    for (int i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * i % MOD;
        powerOfTwo[i] = powerOfTwo[i - 1] * 2 % MOD;
    }

    // Nghịch đảo giai thừa: tính (n!)^{-1} bằng Fermat rồi truy hồi ngược
    inverseFactorial[n] = modPow(factorial[n], MOD - 2);
    for (int i = n; i >= 1; --i) {
        inverseFactorial[i - 1] = inverseFactorial[i] * i % MOD;
    }

    // Tổ hợp C(total, chosen) trong O(1)
    const auto combination = [&](int total, int chosen) -> long long {
        if (chosen < 0 || chosen > total) {
            return 0;
        }
        return factorial[total] * inverseFactorial[chosen] % MOD
               * inverseFactorial[total - chosen] % MOD;
    };

    // Bao hàm - loại trừ: bắt đầu từ n! (không ép cạnh nào)
    long long answer = factorial[n];

    // Chọn "edges" cạnh (cặp i, i+1 phải kề nhau) trong chuỗi 1--2--...--n
    for (int edges = 1; edges < n; ++edges) {
        long long edgeSets = 0;
        const int maxRuns = min(edges, n - edges);

        // "runs" đoạn liên tiếp tạo bởi các cạnh đã chọn
        for (int runs = 1; runs <= maxRuns; ++runs) {
            // Số cách chia edges cạnh thành runs độ dài dương: C(edges-1, runs-1)
            long long ways = combination(edges - 1, runs - 1);
            // Chọn vị trí cho runs đoạn trên n-1 cạnh: C(n-edges, runs)
            ways = ways * combination(n - edges, runs) % MOD;
            // Mỗi đoạn xếp theo chiều tăng hoặc giảm: nhân 2^runs
            ways = ways * powerOfTwo[runs] % MOD;
            edgeSets += ways;
            if (edgeSets >= MOD) {
                edgeSets -= MOD;
            }
        }

        // Sau khi co "edges" cạnh còn n-edges khối: sắp xếp (n-edges)! cách
        const long long term = edgeSets * factorial[n - edges] % MOD;
        // Dấu luân phiên (-1)^edges của bao hàm - loại trừ
        if (edges & 1) {
            answer -= term;
            if (answer < 0) {
                answer += MOD;
            }
        } else {
            answer += term;
            if (answer >= MOD) {
                answer -= MOD;
            }
        }
    }

    cout << answer << '\n';
}
