#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int testCases;
    cin >> testCases;
    while (testCases--) {
        long long leftPile;
        long long rightPile;
        cin >> leftPile >> rightPile;

        const bool divisible = (leftPile + rightPile) % 3 == 0;
        const bool balanced = 2 * leftPile >= rightPile &&
                              2 * rightPile >= leftPile;
        cout << (divisible && balanced ? "YES" : "NO") << '\n';
    }
    return 0;
}
