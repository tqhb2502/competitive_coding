#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 5001;

static long long pre[MAXN + 4];         // prefix sum, đánh chỉ số từ 1
static long long dp[MAXN][MAXN];         // bảng dp đầy đủ (~191 MB)
static int optPrev[MAXN + 4];            // điểm cắt tối ưu cho độ dài L-1 (rolling, bộ nhớ O(n))
static int optCur[MAXN + 4];             // điểm cắt tối ưu cho độ dài L

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    // Prefix sum để lấy trọng số đoạn w(i, j) = pre[j] - pre[i-1] trong O(1).
    for (int i = 1; i <= n; i++) {
        long long x;
        scanf("%lld", &x);
        pre[i] = pre[i - 1] + x;
    }

    // Trường hợp cơ sở: đoạn độ dài 1 -> chi phí 0, quy ước điểm cắt opt = i.
    for (int i = 1; i <= n; i++) {
        dp[i][i] = 0;
        optPrev[i] = i;
    }

    // Interval DP duyệt theo độ dài đoạn len tăng dần.
    for (int len = 2; len <= n; len++) {
        for (int i = 1; i + len - 1 <= n; i++) {
            int j = i + len - 1;
            long long w = pre[j] - pre[i - 1];

            // Knuth optimization: k chỉ cần duyệt trong [opt[i][j-1], opt[i+1][j]].
            // optPrev[i]   = opt của đoạn [i, j-1]   (độ dài len-1)
            // optPrev[i+1] = opt của đoạn [i+1, j]   (độ dài len-1)
            int lo = optPrev[i];
            int hi = min(j - 1, optPrev[i + 1]);   // chặn trên cho trường hợp cơ sở

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
        // Chuyển optCur -> optPrev cho vòng lặp độ dài tiếp theo.
        for (int i = 1; i + len - 1 <= n; i++) optPrev[i] = optCur[i];
    }

    printf("%lld\n", dp[1][n]);
    return 0;
}
