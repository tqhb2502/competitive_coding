// Sliding Window Or - CSES 3405
// https://cses.fi/problemset/task/3405
//
// Sinh mảng a[] bằng công thức x1=x, xi=(a*x[i-1]+b) mod c.
// Với mỗi cửa sổ độ dài k tính OR các phần tử, rồi in XOR của tất cả các OR đó.
// OR không "trừ" được nên ta duy trì cnt[bit] = số phần tử trong cửa sổ có bit đó,
// và giữ giá trị OR (cur) một cách incremental: bật bit khi cnt 0->1, tắt khi cnt 1->0.

#include <bits/stdc++.h>
using namespace std;

static int arr[10000000]; // n <= 10^7

int main() {
    int n, k;
    long long x, a, b, c;
    if (scanf("%d %d", &n, &k) != 2) return 0;
    scanf("%lld %lld %lld %lld", &x, &a, &b, &c);

    // Sinh mảng. Giá trị < 2^30 (c <= 10^9, và x <= 10^9).
    unsigned long long prev = (unsigned long long)x;
    arr[0] = (int)prev;
    unsigned long long ua = (unsigned long long)a;
    unsigned long long ub = (unsigned long long)b;
    unsigned long long uc = (unsigned long long)c;
    for (int i = 1; i < n; ++i) {
        prev = (ua * prev + ub) % uc;
        arr[i] = (int)prev;
    }

    int cnt[32];
    memset(cnt, 0, sizeof(cnt));
    unsigned int cur = 0;  // giá trị OR của cửa sổ hiện tại
    unsigned int ans = 0;  // XOR của tất cả window OR

    for (int i = 0; i < n; ++i) {
        // Thêm arr[i] vào cửa sổ
        int v = arr[i];
        while (v) {
            int bit = __builtin_ctz((unsigned)v);
            if (cnt[bit]++ == 0) cur |= (1u << bit);
            v &= v - 1;
        }
        // Xóa arr[i-k] khi nó rời khỏi cửa sổ
        if (i >= k) {
            int w = arr[i - k];
            while (w) {
                int bit = __builtin_ctz((unsigned)w);
                if (--cnt[bit] == 0) cur &= ~(1u << bit);
                w &= w - 1;
            }
        }
        // Cửa sổ [i-k+1, i] đã đủ k phần tử
        if (i >= k - 1) ans ^= cur;
    }

    printf("%u\n", ans);
    return 0;
}
