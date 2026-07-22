// Transfer Speeds Sum - CSES 3111
// https://cses.fi/problemset/task/3111
//
// Cho một cây (tree). Tốc độ truyền giữa u và v = trọng số NHỎ NHẤT trên đường
// đi duy nhất giữa chúng. Tính tổng trên tất cả các cặp (u, v).
//
// Ý tưởng: xử lý các cạnh theo trọng số GIẢM DẦN, dùng DSU. Khi cạnh trọng số x
// nối hai thành phần kích thước s1, s2 thì mọi cặp chéo (u thuộc A, v thuộc B) có
// bottleneck đúng bằng x (vì các cạnh trong A, B đều > x, và đó là cây nên đường
// đi phải đi qua cạnh này). Đóng góp x * s1 * s2.

#include <bits/stdc++.h>
using namespace std;

int par[200005];
long long sz[200005];

int findp(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]]; // path halving
        x = par[x];
    }
    return x;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    // Cạnh: (trọng số, a, b)
    vector<array<int, 3>> edges;
    edges.reserve(max(0, n - 1));
    for (int i = 0; i < n - 1; ++i) {
        int a, b, x;
        scanf("%d %d %d", &a, &b, &x);
        edges.push_back({x, a, b});
    }

    for (int i = 1; i <= n; ++i) { par[i] = i; sz[i] = 1; }

    // Sắp xếp theo trọng số GIẢM DẦN.
    sort(edges.begin(), edges.end(), [](const array<int,3>& p, const array<int,3>& q) {
        return p[0] > q[0];
    });

    long long ans = 0;
    for (const auto& e : edges) {
        long long w = e[0];
        int ra = findp(e[1]);
        int rb = findp(e[2]);
        // Trong cây hai đầu cạnh luôn ở hai thành phần khác nhau.
        ans += w * sz[ra] * sz[rb];
        // Union by size.
        if (sz[ra] < sz[rb]) swap(ra, rb);
        par[rb] = ra;
        sz[ra] += sz[rb];
    }

    printf("%lld\n", ans);
    return 0;
}
