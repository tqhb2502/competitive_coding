// Monster Game I - CSES 2084
// https://cses.fi/problemset/task/2084
//
// DP: dp[i] = min_{0<=j<i} ( dp[j] + s_i * fac[j] ), with fac[0]=x, dp[0]=0.
//     Answer = dp[n].
// Each candidate j is a line y = fac[j]*X + dp[j]; we query the minimum at X=s_i.
// Slopes fac[j] are non-increasing, query points s_i are non-decreasing
// => monotone Convex Hull Trick (lower envelope).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x;
    cin >> n >> x;

    vector<long long> s(n + 1), f(n + 1);
    for (int i = 1; i <= n; i++) cin >> s[i];
    for (int i = 1; i <= n; i++) cin >> f[i];

    // Lower-envelope lines: y = M[k]*X + B[k], slopes M strictly decreasing.
    vector<long long> M, B;

    // l2 is redundant given neighbours l1 (left) and l3 (right).
    // Slopes strictly decreasing => (M[l1]-M[l2]) > 0 and (M[l1]-M[l3]) > 0.
    // Condition: intersect_x(l1,l3) <= intersect_x(l1,l2).
    auto bad = [&](int l1, int l2, int l3) -> bool {
        return (__int128)(B[l3] - B[l1]) * (M[l1] - M[l2])
             <= (__int128)(B[l2] - B[l1]) * (M[l1] - M[l3]);
    };

    auto add = [&](long long m, long long b) {
        int sz = (int)M.size();
        // Equal slope with current last line: keep the one with smaller intercept.
        if (sz > 0 && M[sz - 1] == m) {
            if (B[sz - 1] <= b) return;      // existing line dominates the new one
            M.pop_back();
            B.pop_back();
        }
        M.push_back(m);
        B.push_back(b);
        while ((int)M.size() >= 3) {
            int t = (int)M.size();
            if (bad(t - 3, t - 2, t - 1)) {
                // remove the middle line (index t-2)
                M[t - 2] = M[t - 1];
                B[t - 2] = B[t - 1];
                M.pop_back();
                B.pop_back();
            } else break;
        }
    };

    auto query = [&](long long q) -> long long {
        int lo = 0, hi = (int)M.size() - 1;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (M[mid] * q + B[mid] > M[mid + 1] * q + B[mid + 1]) lo = mid + 1;
            else hi = mid;
        }
        return M[lo] * q + B[lo];
    };

    // Initial state: line with slope x, intercept dp[0]=0.
    add(x, 0);

    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        long long dp = query(s[i]);
        if (i == n) ans = dp;
        add(f[i], dp);
    }

    cout << ans << "\n";
    return 0;
}
