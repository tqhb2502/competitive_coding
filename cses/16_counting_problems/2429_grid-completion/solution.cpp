#include <algorithm>
#include <iostream>

using namespace std;

constexpr int MOD = 1'000'000'007;
constexpr int MAX_N = 500;

// Vị trí A/B của mỗi hàng và mỗi cột (-1 nghĩa là chưa có).
int rowA[MAX_N];
int rowB[MAX_N];
int columnA[MAX_N];
int columnB[MAX_N];
long long factorial[MAX_N + 1];
long long inverseFactorial[MAX_N + 1];
// Các tổng con SA[u], SB[u] theo số sự kiện trùng u (xem idea.txt).
long long remainingA[MAX_N + 1];
long long remainingB[MAX_N + 1];

// Luỹ thừa nhanh theo modulo, dùng để lấy nghịch đảo qua Fermat.
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

    fill(rowA, rowA + n, -1);
    fill(rowB, rowB + n, -1);
    fill(columnA, columnA + n, -1);
    fill(columnB, columnB + n, -1);
    int placedA = 0;
    int placedB = 0;

    // Đọc lưới và ghi lại vị trí các chữ cái A, B đã cho trước.
    for (int row = 0; row < n; ++row) {
        char line[MAX_N + 1];
        cin >> line;
        for (int column = 0; column < n; ++column) {
            if (line[column] == 'A') {
                rowA[row] = column;
                columnA[column] = row;
                ++placedA;
            } else if (line[column] == 'B') {
                rowB[row] = column;
                columnB[column] = row;
                ++placedB;
            }
        }
    }

    // Phân loại các sự kiện trùng ô (A và B cùng một ô) theo ba loại.
    int rowsFreeForBoth = 0;      // x: hàng chưa có cả A lẫn B
    int columnsFreeForBoth = 0;   // y: cột chưa có cả A lẫn B
    int fixedAOverlapChoices = 0; // zA: hàng có A, cột của A chưa có B
    int fixedBOverlapChoices = 0; // zB: hàng có B, cột của B chưa có A

    for (int row = 0; row < n; ++row) {
        if (rowA[row] == -1 && rowB[row] == -1) {
            ++rowsFreeForBoth;
        } else if (rowA[row] != -1 && rowB[row] == -1
                   && columnB[rowA[row]] == -1) {
            ++fixedAOverlapChoices;
        } else if (rowA[row] == -1 && rowB[row] != -1
                   && columnA[rowB[row]] == -1) {
            ++fixedBOverlapChoices;
        }
    }

    for (int column = 0; column < n; ++column) {
        if (columnA[column] == -1 && columnB[column] == -1) {
            ++columnsFreeForBoth;
        }
    }

    // Tiền xử lý giai thừa và nghịch đảo giai thừa để tính tổ hợp nCr.
    factorial[0] = 1;
    inverseFactorial[0] = 1;
    for (int i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * i % MOD;
    }
    inverseFactorial[n] = modPow(factorial[n], MOD - 2);
    for (int i = n; i >= 1; --i) {
        inverseFactorial[i - 1] = inverseFactorial[i] * i % MOD;
    }

    const auto combination = [&](int total, int chosen) -> long long {
        if (chosen < 0 || chosen > total) {
            return 0;
        }
        return factorial[total] * inverseFactorial[chosen] % MOD
               * inverseFactorial[total - chosen] % MOD;
    };

    // Số A, số B còn phải điền và số sự kiện trùng tối đa của loại K(x, y).
    const int missingA = n - placedA;
    const int missingB = n - placedB;
    const int maxCommon = min(rowsFreeForBoth, columnsFreeForBoth);
    for (int common = 0; common <= maxCommon; ++common) {
        // SA[common]: tổng bao hàm - loại trừ trên các sự kiện cô lập loại zB,
        // phần còn lại của hoán vị A là (missingA - common - chosen)!.
        for (int chosen = 0;
             chosen <= fixedBOverlapChoices && chosen + common <= missingA;
             ++chosen) {
            long long term = combination(fixedBOverlapChoices, chosen)
                             * factorial[missingA - common - chosen] % MOD;
            if (chosen & 1) {
                remainingA[common] -= term;
                if (remainingA[common] < 0) {
                    remainingA[common] += MOD;
                }
            } else {
                remainingA[common] += term;
                if (remainingA[common] >= MOD) {
                    remainingA[common] -= MOD;
                }
            }
        }

        // SB[common]: tương tự cho các sự kiện cô lập loại zA, phần còn lại của
        // hoán vị B là (missingB - common - chosen)!.
        for (int chosen = 0;
             chosen <= fixedAOverlapChoices && chosen + common <= missingB;
             ++chosen) {
            long long term = combination(fixedAOverlapChoices, chosen)
                             * factorial[missingB - common - chosen] % MOD;
            if (chosen & 1) {
                remainingB[common] -= term;
                if (remainingB[common] < 0) {
                    remainingB[common] += MOD;
                }
            } else {
                remainingB[common] += term;
                if (remainingB[common] >= MOD) {
                    remainingB[common] -= MOD;
                }
            }
        }
    }

    // Cộng dồn kết quả: tổng theo common của
    //   (-1)^common * C(x, common) * C(y, common) * common! * SA * SB.
    long long answer = 0;
    for (int common = 0; common <= maxCommon; ++common) {
        // Số cách chọn common sự kiện đôi một khác hàng/khác cột trong K(x, y).
        long long matchings = combination(rowsFreeForBoth, common);
        matchings = matchings * combination(columnsFreeForBoth, common) % MOD;
        matchings = matchings * factorial[common] % MOD;

        long long term = matchings * remainingA[common] % MOD;
        term = term * remainingB[common] % MOD;
        if (common & 1) {
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
