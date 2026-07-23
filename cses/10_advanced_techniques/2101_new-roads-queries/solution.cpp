#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    // Lá 1..n là các thành phố ban đầu, thêm tối đa n-1 nút trong -> tổng <= 2n-1
    int maxNodes = 2 * n + 1;
    vector<int> par(maxNodes, 0);       // cha trong cây tái tạo Kruskal (0 = nút giả)
    vector<int> val(maxNodes, 0);       // ngày hợp nhất tại mỗi nút trong; lá giữ 0
    vector<int> f(maxNodes);            // DSU: đại diện (nút cao nhất) hiện tại của thành phần
    for (int i = 0; i < maxNodes; ++i) f[i] = i;

    // Tìm đại diện của DSU, có nén đường (path compression)
    auto find = [&](int x) {
        int r = x;
        while (f[r] != r) r = f[r];
        while (f[x] != r) { int nx = f[x]; f[x] = r; x = nx; }
        return r;
    };

    // Dựng cây tái tạo Kruskal: duyệt các cạnh theo đúng thứ tự ngày 1..m
    int cnt = n;                        // số nút hiện có; mỗi nút trong mới = ++cnt
    for (int d = 1; d <= m; ++d) {
        int a, b;
        cin >> a >> b;
        int ra = find(a), rb = find(b);
        if (ra != rb) {
            // Tạo nút trong mới w làm cha của hai gốc cũ, ghi ngày hợp nhất d
            int w = ++cnt;
            val[w] = d;
            par[ra] = w;
            par[rb] = w;
            f[ra] = w;
            f[rb] = w;
            f[w] = w;
        }
    }

    const int LOG = 20;                 // 2^20 > 2*2e5, đủ cho binary lifting
    vector<int> depth(cnt + 1, 0);
    vector<vector<int>> up(LOG, vector<int>(cnt + 1, 0));

    // Cha luôn có chỉ số lớn hơn con nên duyệt giảm dần để depth[par[v]] có sẵn
    for (int v = cnt; v >= 1; --v) {
        if (par[v] == 0) depth[v] = 0;
        else depth[v] = depth[par[v]] + 1;
        up[0][v] = par[v];
    }
    // Bảng nhảy: tổ tiên thứ 2^k = đi 2^(k-1) rồi lại 2^(k-1)
    for (int k = 1; k < LOG; ++k)
        for (int v = 1; v <= cnt; ++v)
            up[k][v] = up[k - 1][up[k - 1][v]];

    // LCA chuẩn: đưa hai nút về cùng depth rồi cùng nhảy lên trên
    auto lca = [&](int a, int b) -> int {
        if (depth[a] < depth[b]) swap(a, b);
        int diff = depth[a] - depth[b];
        for (int k = 0; k < LOG; ++k)
            if (diff & (1 << k)) a = up[k][a];
        if (a == b) return a;
        for (int k = LOG - 1; k >= 0; --k)
            if (up[k][a] != up[k][b]) { a = up[k][a]; b = up[k][b]; }
        return par[a];
    };

    // Trả lời truy vấn: khác cây -> không bao giờ nối (-1); ngược lại val[LCA(a,b)]
    string out;
    out.reserve(q * 3);
    for (int i = 0; i < q; ++i) {
        int a, b;
        cin >> a >> b;
        if (find(a) != find(b)) {
            out += "-1\n";
        } else {
            out += to_string(val[lca(a, b)]);
            out += '\n';
        }
    }
    cout << out;
    return 0;
}
