// Knuth Division - CSES 2088
// https://cses.fi/problemset/task/2088
//
// Chia mang thanh cac phan tu don le bang cac lan cat; moi lan cat mot doan con
// chi phi bang tong doan con do. Toi thieu hoa tong chi phi.
//
// dp[i][j] = w(i,j) + min_{i<=k<j} ( dp[i][k] + dp[k+1][j] ),  dp[i][i] = 0
// voi w(i,j) = x_i + ... + x_j la tong doan (prefix sum).
// Ham w don dieu va thoa quadrangle inequality => Knuth optimization:
//     opt[i][j-1] <= opt[i][j] <= opt[i+1][j]
// cho toc do O(n^2) thay vi O(n^3).

#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 5001;

static long long pre[MAXN + 4];         // prefix sum, 1-indexed
static long long dp[MAXN][MAXN];         // dp[i][j], full table (~191 MB)
static int optPrev[MAXN + 4];            // opt cho do dai L-1 (rolling, O(n) bo nho)
static int optCur[MAXN + 4];             // opt cho do dai L

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 1; i <= n; i++) {
        long long x;
        scanf("%lld", &x);
        pre[i] = pre[i - 1] + x;
    }

    // Truong hop co so: doan do dai 1 -> chi phi 0, quy uoc opt = i.
    for (int i = 1; i <= n; i++) {
        dp[i][i] = 0;
        optPrev[i] = i;
    }

    for (int len = 2; len <= n; len++) {
        for (int i = 1; i + len - 1 <= n; i++) {
            int j = i + len - 1;
            long long w = pre[j] - pre[i - 1];

            // Knuth: k chi can duyet trong [opt[i][j-1], opt[i+1][j]].
            // optPrev[i]   = opt cua doan [i, j-1]   (do dai len-1)
            // optPrev[i+1] = opt cua doan [i+1, j]   (do dai len-1)
            int lo = optPrev[i];
            int hi = min(j - 1, optPrev[i + 1]);   // chan tren cho truong hop co so

            long long best = LLONG_MAX;
            int bk = lo;
            for (int k = lo; k <= hi; k++) {
                long long v = dp[i][k] + dp[k + 1][j];
                if (v < best) {
                    best = v;
                    bk = k;
                }
            }
            dp[i][j] = best + w;
            optCur[i] = bk;
        }
        // Chuyen optCur -> optPrev cho vong lap do dai tiep theo.
        for (int i = 1; i + len - 1 <= n; i++) optPrev[i] = optCur[i];
    }

    printf("%lld\n", dp[1][n]);
    return 0;
}
