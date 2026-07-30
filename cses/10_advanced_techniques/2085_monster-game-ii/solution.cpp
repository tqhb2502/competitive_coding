#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

static const ll INF = (ll)4e18;       // tung độ gốc "+vô cực" cho đường rỗng
static const int MAXX = 1000000;      // miền [0, MAXX] phủ mọi s_i trong [1, 1e6]

// Một đường thẳng g(t) = m*t + b
struct Line {
    ll m, b;
    ll eval(ll t) const { return m * t + b; }
};

// Li Chao tree (dynamic convex hull trick) trên miền cố định [0, MAXX].
// Mỗi node giữ đường thẳng tối ưu nhất tại điểm giữa của đoạn.
struct LiChao {
    int LO, HI;
    vector<Line> seg;
    LiChao(int lo, int hi) : LO(lo), HI(hi) {
        // Khởi tạo mọi node bằng đường vô cực để đại diện tập rỗng.
        seg.assign(4 * (hi - lo + 1), Line{0, INF});
    }
    // Chèn một đường thẳng: đẩy đường thua xuống nửa đoạn phù hợp (O(log)).
    void add(Line nw, int node, int l, int r) {
        int mid = l + (r - l) / 2;
        bool onLeft = nw.eval(l)   < seg[node].eval(l);
        bool onMid  = nw.eval(mid) < seg[node].eval(mid);
        if (onMid) swap(seg[node], nw);   // giữ đường tốt hơn tại điểm giữa
        if (l == r) return;
        if (onLeft != onMid) add(nw, node * 2,     l,       mid);
        else                 add(nw, node * 2 + 1, mid + 1, r);
    }
    void add(Line nw) { add(nw, 1, LO, HI); }
    // Truy vấn giá trị nhỏ nhất của mọi đường đã chèn tại điểm x.
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

    // Duyệt theo thứ tự: query dp[i] rồi chèn đường của i cho các bước sau.
    ll ans = 0;
    for (int i = 1; i <= n; ++i) {
        ll dpi = tree.query((int)s[i]);           // dp[i] = min các đường j < i
        if (i == n) { ans = dpi; break; }         // bắt buộc đánh con quái cuối n
        tree.add(Line{f[i], dpi});                // đường (slope = f_i, intercept = dp[i])
    }

    cout << ans << '\n';
    return 0;
}
