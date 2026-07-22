// Sliding Window Minimum - CSES 3221
// https://cses.fi/problemset/task/3221
//
// Sinh mảng theo công thức x_1 = x, x_i = (a*x_{i-1} + b) mod c.
// Với mỗi cửa sổ độ dài k, tính giá trị nhỏ nhất, rồi in ra XOR của tất cả
// các giá trị nhỏ nhất đó.
//
// Kỹ thuật: monotonic deque (hàng đợi đơn điệu tăng) -> O(n) thời gian.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;

    long long x, a, b, c;
    scanf("%lld %lld %lld %lld", &x, &a, &b, &c);

    // Sinh mảng. x_1 = x giữ nguyên (có thể tới 10^9, vẫn vừa int).
    // a * x_{i-1} có thể tới ~10^18 nên tính bằng long long.
    vector<int> arr(n);
    long long cur = x;                 // x_1 = x
    arr[0] = (int)cur;
    for (int i = 1; i < n; i++) {
        cur = (a * cur + b) % c;       // x_i = (a*x_{i-1} + b) mod c
        arr[i] = (int)cur;
    }

    // Monotonic deque lưu chỉ số, giá trị arr[idx] tăng dần từ front -> back.
    // Deque được cài bằng mảng int cấp phát sẵn để tối ưu tốc độ.
    vector<int> dq(n);
    int head = 0, tail = 0;            // vùng hợp lệ: [head, tail)
    long long ans = 0;

    for (int i = 0; i < n; i++) {
        // (1) Loại khỏi front các chỉ số đã ra khỏi cửa sổ (index <= i - k).
        if (head < tail && dq[head] <= i - k) head++;
        // (2) Loại khỏi back các phần tử >= arr[i] (không còn có thể là min).
        while (head < tail && arr[dq[tail - 1]] >= arr[i]) tail--;
        // (3) Đẩy i vào back.
        dq[tail++] = i;
        // (4) Khi cửa sổ đủ dài, front là min của cửa sổ -> XOR vào kết quả.
        if (i >= k - 1) ans ^= arr[dq[head]];
    }

    printf("%lld\n", ans);
    return 0;
}
