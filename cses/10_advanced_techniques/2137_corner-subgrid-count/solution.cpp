// Corner Subgrid Count - CSES 2137
// https://cses.fi/problemset/task/2137
//
// Y tuong: voi moi cap hang (i, j), goi k = so cot ma ca hai hang deu la o den.
// Moi cach chon 2 cot phan biet trong k cot do tao thanh mot corner subgrid dep,
// nen cong C(k, 2) = k*(k-1)/2 vao dap an. Dung bitset de tinh k = popcount cua
// (row[i] AND row[j]) trong O(n/64). Tong do phuc tap O(n^3 / 64).

#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 3000;
static bitset<MAXN> rows[MAXN];

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    static char buf[MAXN + 5];
    for (int i = 0; i < n; i++) {
        scanf("%s", buf);
        for (int j = 0; j < n; j++) {
            if (buf[j] == '1') rows[i].set(j);
        }
    }

    long long ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long k = (rows[i] & rows[j]).count();
            ans += k * (k - 1) / 2;
        }
    }

    printf("%lld\n", ans);
    return 0;
}
