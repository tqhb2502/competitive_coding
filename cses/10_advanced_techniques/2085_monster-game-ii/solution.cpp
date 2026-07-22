// Monster Game II — CSES 2085
// https://cses.fi/problemset/task/2085
//
// dp[i] = min_{0<=j<i} ( dp[j] + f_j * s_i ),  dp[0]=0, f_0 = x.
// Answer = dp[n]. Each j gives a line g_j(t) = f_j * t + dp[j], and
// dp[i] = min_j g_j(s_i). Slopes f_j and query points s_i are NOT monotonic,
// so we use a Li Chao tree (dynamic convex hull trick) for min queries.

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

static const ll INF = (ll)4e18;      // "+infinity" intercept for empty lines
static const int MAXX = 1000000;      // domain [0, MAXX] covers all s_i in [1,1e6]

struct Line {
    ll m, b;                          // value at t: m*t + b
    ll eval(ll t) const { return m * t + b; }
};

// Li Chao tree over the fixed domain [0, MAXX].
struct LiChao {
    int LO, HI;
    vector<Line> seg;
    LiChao(int lo, int hi) : LO(lo), HI(hi) {
        seg.assign(4 * (hi - lo + 1), Line{0, INF});
    }
    void add(Line nw, int node, int l, int r) {
        int mid = l + (r - l) / 2;
        bool onLeft = nw.eval(l)   < seg[node].eval(l);
        bool onMid  = nw.eval(mid) < seg[node].eval(mid);
        if (onMid) swap(seg[node], nw);
        if (l == r) return;
        if (onLeft != onMid) add(nw, node * 2,     l,       mid);
        else                 add(nw, node * 2 + 1, mid + 1, r);
    }
    void add(Line nw) { add(nw, 1, LO, HI); }
    ll query(int x, int node, int l, int r) const {
        ll res = seg[node].eval(x);
        if (l == r) return res;
        int mid = l + (r - l) / 2;
        if (x <= mid) return min(res, query(x, node * 2,     l,       mid));
        else          return min(res, query(x, node * 2 + 1, mid + 1, r));
    }
    ll query(int x) const { return query(x, 1, LO, HI); }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    ll x;
    if (!(cin >> n >> x)) return 0;

    vector<ll> s(n + 1), f(n + 1);
    for (int i = 1; i <= n; ++i) cin >> s[i];
    for (int i = 1; i <= n; ++i) cin >> f[i];

    LiChao tree(0, MAXX);
    tree.add(Line{x, 0});             // j = 0: slope = x, intercept = dp[0] = 0

    ll ans = 0;
    for (int i = 1; i <= n; ++i) {
        ll dpi = tree.query((int)s[i]);
        if (i == n) { ans = dpi; break; }
        tree.add(Line{f[i], dpi});    // line for future queries
    }

    cout << ans << '\n';
    return 0;
}
