#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    long long total = 0;
    // Đếm số lượng số trong [0, n] có bit i bật, dựa trên tính tuần hoàn của bit đó.
    // 2^50 > 10^15, nên duyệt tới bit 50 là đủ cho mọi n trong ràng buộc.
    for (int i = 0; i <= 50; ++i) {
        long long half = 1LL << i;         // 2^i: số phần tử mỗi nửa chu kỳ
        long long cycle = half << 1;       // 2^(i+1): độ dài một chu kỳ đầy đủ
        long long cnt = (n + 1) / cycle;   // số chu kỳ đầy đủ trong (n + 1) số
        long long ones = cnt * half;       // mỗi chu kỳ đầy đủ đóng góp 2^i số có bit bật
        long long rem = (n + 1) % cycle;   // phần dư còn lại sau các chu kỳ đầy đủ
        ones += max(0LL, rem - half);      // phần dư đóng góp thêm max(0, rem - 2^i)
        total += ones;                     // cộng dồn số bit 1 tại vị trí bit i
    }

    cout << total << '\n';
    return 0;
}
