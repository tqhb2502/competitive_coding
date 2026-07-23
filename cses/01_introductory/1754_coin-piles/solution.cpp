#include <iostream>

using namespace std;

int main() {
    // Tăng tốc I/O vì có tối đa 1e5 truy vấn
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int testCases;
    cin >> testCases;
    while (testCases--) {
        long long leftPile;
        long long rightPile;
        cin >> leftPile >> rightPile;

        // Điều kiện (a + b) chia hết cho 3 để x, y là số nguyên
        const bool divisible = (leftPile + rightPile) % 3 == 0;
        // Điều kiện x >= 0 và y >= 0: hai đống không quá chênh lệch
        const bool balanced = 2 * leftPile >= rightPile &&
                              2 * rightPile >= leftPile;
        // Đưa được về (0, 0) khi và chỉ khi cả hai điều kiện đúng
        cout << (divisible && balanced ? "YES" : "NO") << '\n';
    }
    return 0;
}
