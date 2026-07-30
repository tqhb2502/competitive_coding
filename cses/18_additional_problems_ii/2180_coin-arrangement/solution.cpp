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

    // topBalance, bottomBalance: số xu dư tích lũy ở mỗi hàng (âm nghĩa là thiếu).
    long long topBalance = 0;
    long long bottomBalance = 0;
    long long answer = 0;
    // Quét các cột từ trái sang phải theo kiểu tham lam (greedy).
    for (int column = 0; column < n; ++column) {
        // Cộng (số xu của ô trừ 1) vào số dư của hàng tương ứng.
        topBalance += topRow[column] - 1;
        bottomBalance += bottomRow[column] - 1;

        // Hai số dư trái dấu: chuyển dọc để triệt tiêu ngay tại cột này.
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

        // Số dư còn lại bắt buộc đi qua biên phải sang cột kế tiếp.
        if (column + 1 < n) {
            answer += llabs(topBalance) + llabs(bottomBalance);
        }
    }

    cout << answer << '\n';
}
