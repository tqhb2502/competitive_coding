#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;

    // Đóng gói (pack) mỗi xâu nhị phân độ dài k <= 30 vào một số nguyên
    // 32 bit theo thứ tự MSB-first (ký tự đầu ở bit cao nhất); mỗi ký tự
    // chiếm đúng một bit riêng.
    vector<unsigned int> a(n);
    char buf[40];
    for (int i = 0; i < n; i++) {
        scanf("%s", buf);
        unsigned int x = 0;
        for (int j = 0; j < k; j++)
            x = (x << 1) | (unsigned int)(buf[j] - '0');
        a[i] = x;
    }

    // Cận trên: hai xâu độ dài k khác nhau ở nhiều nhất k vị trí.
    int ans = k;
    // Duyệt mọi cặp (i, j) với i < j; khoảng cách Hamming của cặp bằng
    // popcount(a[i] XOR a[j]) — số bit khác nhau giữa hai xâu.
    for (int i = 0; i < n && ans > 0; i++) {
        unsigned int ai = a[i];
        for (int j = i + 1; j < n; j++) {
            int d = __builtin_popcount(ai ^ a[j]);
            if (d < ans) {
                ans = d;
                if (ans == 0) break;   // trùng nhau hoàn toàn, dừng sớm
            }
        }
    }

    printf("%d\n", ans);
    return 0;
}
