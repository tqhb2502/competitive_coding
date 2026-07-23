#include <bits/stdc++.h>
using namespace std;

int par[200005];
long long sz[200005];

// Tìm gốc thành phần với path halving.
int findp(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    // Mỗi cạnh lưu dưới dạng (trọng số, a, b).
    vector<array<int, 3>> edges;
    edges.reserve(max(0, n - 1));
    for (int i = 0; i < n - 1; ++i) {
        int a, b, x;
        scanf("%d %d %d", &a, &b, &x);
        edges.push_back({x, a, b});
    }

    // Khởi tạo DSU: mỗi đỉnh là một thành phần riêng.
    for (int i = 1; i <= n; ++i) { par[i] = i; sz[i] = 1; }

    // Xử lý các cạnh theo trọng số GIẢM DẦN (bottleneck path).
    sort(edges.begin(), edges.end(), [](const array<int,3>& p, const array<int,3>& q) {
        return p[0] > q[0];
    });

    long long ans = 0;
    for (const auto& e : edges) {
        long long w = e[0];
        int ra = findp(e[1]);
        int rb = findp(e[2]);
        // Vì là cây, hai đầu cạnh luôn ở hai thành phần khác nhau. Với cạnh trọng số
        // x nối A (kích thước s1) và B (kích thước s2), mọi cặp chéo có bottleneck
        // đúng bằng x, nên cạnh đóng góp x * s1 * s2 vào tổng đáp số.
        ans += w * sz[ra] * sz[rb];
        // Hợp nhất hai thành phần theo union by size.
        if (sz[ra] < sz[rb]) swap(ra, rb);
        par[rb] = ra;
        sz[ra] += sz[rb];
    }

    printf("%lld\n", ans);
    return 0;
}
