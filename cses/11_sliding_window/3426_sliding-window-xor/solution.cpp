#include <bits/stdc++.h>
using namespace std;

static int arr[10000005];

int main() {
    int n, k;
    long long x, a, b, c;
    if (scanf("%d %d", &n, &k) != 2) return 0;
    if (scanf("%lld %lld %lld %lld", &x, &a, &b, &c) != 4) return 0;

    // Sinh mảng theo công thức truy hồi: x_1 = x giữ nguyên, các phần tử sau lấy
    // mod c. Tích a * prev có thể tới 10^18 nên phải dùng long long khi nhân.
    long long prev = x;
    arr[0] = (int)prev;
    for (int i = 1; i < n; i++) {
        prev = (a * prev + b) % c;
        arr[i] = (int)prev;
    }

    // Cửa sổ đầu tiên [0, k-1]: tính trực tiếp rồi khởi tạo đáp án.
    int wx = 0;
    for (int i = 0; i < k; i++) wx ^= arr[i];
    int ans = wx;

    // Trượt cửa sổ: XOR tự nghịch đảo nên chỉ cần bỏ arr[i-k] (triệt tiêu) và
    // thêm arr[i]; sau mỗi bước XOR window-xor vào đáp án tổng.
    for (int i = k; i < n; i++) {
        wx ^= arr[i - k] ^ arr[i];
        ans ^= wx;
    }

    printf("%d\n", ans);
    return 0;
}
