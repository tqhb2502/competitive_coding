#include <algorithm>
#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;
    // Trung vị là phần tử thứ target trong dãy đã sắp xếp (n lẻ)
    const long long target = (n * n + 1) / 2;

    // Tìm nhị phân theo giá trị: giá trị nhỏ nhất sao cho có >= target phần tử <= nó
    long long low = 1;
    long long high = n * n;
    while (low < high) {
        long long middle = low + (high - low) / 2;

        // count = số phần tử của bảng nhân không vượt quá middle
        long long count = 0;
        for (long long row = 1; row <= n; ++row) {
            // Hàng row có min(n, floor(middle/row)) phần tử <= middle
            count += min(n, middle / row);
        }

        // count không giảm theo middle nên thu hẹp khoảng tìm nhị phân
        if (count >= target) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    cout << low << '\n';
}
