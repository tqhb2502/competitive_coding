// Counting Bits
// https://cses.fi/problemset/task/1146
//
// Đếm tổng số bit 1 trong biểu diễn nhị phân của các số từ 1 tới n.
// Với mỗi vị trí bit i, số lượng số trong [0, n] có bit i bật được tính
// dựa trên tính tuần hoàn (chu kỳ cycle = 2^(i+1)) của bit đó.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    long long total = 0;
    // 2^50 > 10^15, nên duyệt tới bit 50 là đủ cho mọi n trong ràng buộc.
    for (int i = 0; i <= 50; ++i) {
        long long half = 1LL << i;        // 2^i
        long long cycle = half << 1;       // 2^(i+1)
        long long cnt = (n + 1) / cycle;   // số chu kỳ đầy đủ
        long long ones = cnt * half;       // mỗi chu kỳ đóng góp 2^i số có bit bật
        long long rem = (n + 1) % cycle;   // phần dư
        ones += max(0LL, rem - half);      // phần dư đóng góp thêm max(0, rem - 2^i)
        total += ones;
    }

    cout << total << '\n';
    return 0;
}
