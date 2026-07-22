// Sliding Window Xor - CSES 3426
// https://cses.fi/problemset/task/3426
//
// Đề: mảng n phần tử được SINH ra bằng công thức truy hồi:
//     x_1 = x, x_i = (a * x_{i-1} + b) mod c.
// Với mỗi cửa sổ độ dài k, tính XOR các phần tử; rồi lấy XOR của tất cả
// các window-xor đó và in kết quả.
//
// Kỹ thuật: sliding window + prefix xor. XOR có tính tự nghịch đảo nên khi
// cửa sổ trượt ta chỉ bỏ phần tử rời đi và thêm phần tử mới. O(n) time.

#include <bits/stdc++.h>
using namespace std;

static int arr[10000005];

int main() {
    int n, k;
    long long x, a, b, c;
    if (scanf("%d %d", &n, &k) != 2) return 0;
    if (scanf("%lld %lld %lld %lld", &x, &a, &b, &c) != 4) return 0;

    // Sinh mảng: x_1 = x giữ nguyên, các phần tử sau lấy mod c.
    // a * prev có thể tới 10^18 -> phải dùng long long khi nhân.
    long long prev = x;
    arr[0] = (int)prev;
    for (int i = 1; i < n; i++) {
        prev = (a * prev + b) % c;
        arr[i] = (int)prev;
    }

    // Cửa sổ đầu tiên [0, k-1].
    int wx = 0;
    for (int i = 0; i < k; i++) wx ^= arr[i];
    int ans = wx;

    // Trượt cửa sổ: bỏ arr[i-k], thêm arr[i]; XOR window-xor vào ans.
    for (int i = k; i < n; i++) {
        wx ^= arr[i - k] ^ arr[i];
        ans ^= wx;
    }

    printf("%d\n", ans);
    return 0;
}
