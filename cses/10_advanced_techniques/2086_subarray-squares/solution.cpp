// Subarray Squares — CSES 2086
// https://cses.fi/problemset/task/2086
//
// Chia mảng n phần tử dương thành đúng k đoạn con liên tiếp; chi phí mỗi đoạn là
// bình phương tổng của đoạn. Tối thiểu hoá tổng chi phí.
//
// dp[i][j] = min_{m in [i-1, j-1]} dp[i-1][m] + (pre[j]-pre[m])^2
// Hàm chi phí thoả điều kiện Monge (vì pre tăng ngặt) => opt[i][j] không giảm
// theo j => dùng Divide and Conquer DP optimization: O(k * n log n).

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll INF = LLONG_MAX / 4;

int n, k;
vector<ll> pre;      // prefix sums, size n+1
vector<ll> dpPrev;   // lớp i-1
vector<ll> dpCur;    // lớp i

static inline ll cost(int m, int j) {
    ll d = pre[j] - pre[m];
    return d * d;
}

// Tính dpCur[j] cho j in [jl, jr], biết opt-m nằm trong [ml, mr].
void solve(int jl, int jr, int ml, int mr) {
    if (jl > jr) return;
    int jm = (jl + jr) / 2;
    ll best = INF;
    int bestM = ml;
    int hi = min(jm - 1, mr);   // yêu cầu m < jm
    for (int m = ml; m <= hi; ++m) {
        if (dpPrev[m] >= INF) continue;
        ll val = dpPrev[m] + cost(m, jm);
        if (val < best) { best = val; bestM = m; }
    }
    dpCur[jm] = best;
    solve(jl, jm - 1, ml, bestM);
    solve(jm + 1, jr, bestM, mr);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> n >> k)) return 0;
    pre.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        ll x;
        cin >> x;
        pre[i] = pre[i - 1] + x;
    }

    // dp[0]: chỉ dp[0][0] = 0, còn lại = INF
    dpPrev.assign(n + 1, INF);
    dpPrev[0] = 0;
    dpCur.assign(n + 1, INF);

    for (int i = 1; i <= k; ++i) {
        fill(dpCur.begin(), dpCur.end(), INF);
        // j chạy trong [i, n]; m tối ưu trong [i-1, n-1]
        solve(i, n, i - 1, n - 1);
        swap(dpPrev, dpCur);
    }

    cout << dpPrev[n] << "\n";
    return 0;
}
