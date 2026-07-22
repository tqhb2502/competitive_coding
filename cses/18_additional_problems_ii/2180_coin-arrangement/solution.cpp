#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> topRow(n);
    vector<long long> bottomRow(n);
    for (long long& coins : topRow) {
        cin >> coins;
    }
    for (long long& coins : bottomRow) {
        cin >> coins;
    }

    long long topBalance = 0;
    long long bottomBalance = 0;
    long long answer = 0;
    for (int column = 0; column < n; ++column) {
        topBalance += topRow[column] - 1;
        bottomBalance += bottomRow[column] - 1;

        if (topBalance > 0 && bottomBalance < 0) {
            const long long moved = min(topBalance, -bottomBalance);
            topBalance -= moved;
            bottomBalance += moved;
            answer += moved;
        } else if (topBalance < 0 && bottomBalance > 0) {
            const long long moved = min(-topBalance, bottomBalance);
            topBalance += moved;
            bottomBalance -= moved;
            answer += moved;
        }

        if (column + 1 < n) {
            answer += llabs(topBalance) + llabs(bottomBalance);
        }
    }

    cout << answer << '\n';
}
