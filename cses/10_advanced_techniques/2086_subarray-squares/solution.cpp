#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll INF = LLONG_MAX / 4;

int n, k;
vector<ll> pre;      // tổng tiền tố (prefix sum), kích thước n+1
vector<ll> dpPrev;   // lớp i-1
vector<ll> dpCur;    // lớp i

// Chi phí của đoạn (m, j]: bình phương tổng các phần tử trong đoạn.
static inline ll cost(int m, int j) {
    ll d = pre[j] - pre[m];
    return d * d;
}

// Divide and conquer DP optimization cho một lớp i cố định:
// tính dpCur[j] cho j thuộc [jl, jr], biết điểm cắt tối ưu m nằm trong [ml, mr].
void solve(int jl, int jr, int ml, int mr) {
    if (jl > jr) return;
    int jm = (jl + jr) / 2;
    // Quét mọi m hợp lệ để tìm dpCur[jm] và vị trí tối ưu bestM.
    ll best = INF;
    int bestM = ml;
    int hi = min(jm - 1, mr);   // yêu cầu m < jm
    for (int m = ml; m <= hi; ++m) {
        if (dpPrev[m] >= INF) continue;
        ll val = dpPrev[m] + cost(m, jm);
        if (val < best) { best = val; bestM = m; }
    }
    dpCur[jm] = best;
    // Đệ quy: nửa trái dùng opt trong [ml, bestM], nửa phải trong [bestM, mr].
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

    // Cơ sở lớp 0: chỉ dp[0][0] = 0, còn lại = INF.
    dpPrev.assign(n + 1, INF);
    dpPrev[0] = 0;
    dpCur.assign(n + 1, INF);

    // Xây từng lớp i (số đoạn) từ lớp i-1, chỉ giữ hai hàng để tiết kiệm bộ nhớ.
    for (int i = 1; i <= k; ++i) {
        fill(dpCur.begin(), dpCur.end(), INF);
        // j chạy trong [i, n]; m tối ưu trong [i-1, n-1].
        solve(i, n, i - 1, n - 1);
        swap(dpPrev, dpCur);
    }

    cout << dpPrev[n] << "\n";
    return 0;
}
