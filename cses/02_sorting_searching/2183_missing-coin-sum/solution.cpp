#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<long long> coins(n);
    for (long long& coin : coins) {
        cin >> coin;
    }

    // Sắp xếp các đồng xu tăng dần để áp dụng greedy.
    sort(coins.begin(), coins.end());

    // smallestMissing = tổng nhỏ nhất hiện chưa tạo được; bất biến: mọi tổng
    // trong [1, smallestMissing - 1] đều tạo được bằng các đồng đã xét.
    long long smallestMissing = 1;
    for (const long long coin : coins) {
        // Nếu đồng > smallestMissing thì có khoảng trống -> dừng, đó là đáp án.
        if (coin > smallestMissing) {
            break;
        }
        // Ngược lại hai đoạn nối liền, mở rộng phạm vi tạo được.
        smallestMissing += coin;
    }

    cout << smallestMissing << '\n';
    return 0;
}
