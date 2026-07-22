// MST Edge Check - CSES 3407
// https://cses.fi/problemset/task/3407
//
// Với mỗi cạnh, cạnh đó có thể nằm trong MỘT MST nào đó khi và chỉ khi
// hai đầu mút chưa được nối bằng các cạnh trọng số NHỎ HƠN HẲN w.
// Kruskal + DSU: xử lý các cạnh theo nhóm trọng số bằng nhau, kiểm tra
// cả nhóm trước rồi mới union.

#include <bits/stdc++.h>
using namespace std;

static int par[100005], rnk[100005];

int findp(int x) {
    while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; }
    return x;
}

void unite(int a, int b) {
    a = findp(a); b = findp(b);
    if (a == b) return;
    if (rnk[a] < rnk[b]) swap(a, b);
    par[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
}

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<int> a(m), b(m), w(m), idx(m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &a[i], &b[i], &w[i]);
        idx[i] = i;
    }

    sort(idx.begin(), idx.end(), [&](int x, int y) { return w[x] < w[y]; });

    for (int i = 1; i <= n; i++) { par[i] = i; rnk[i] = 0; }

    vector<char> ans(m, 0); // 1 = YES, 0 = NO

    int i = 0;
    while (i < m) {
        int j = i;
        while (j < m && w[idx[j]] == w[idx[i]]) j++;
        // Kiểm tra cả nhóm [i, j) trước (DSU chỉ chứa các cạnh trọng số < w).
        for (int k = i; k < j; k++) {
            int e = idx[k];
            ans[e] = (findp(a[e]) != findp(b[e])) ? 1 : 0;
        }
        // Sau đó mới union toàn bộ nhóm.
        for (int k = i; k < j; k++) {
            int e = idx[k];
            unite(a[e], b[e]);
        }
        i = j;
    }

    string out;
    out.reserve((size_t)m * 4);
    for (int k = 0; k < m; k++) out += (ans[k] ? "YES\n" : "NO\n");
    fputs(out.c_str(), stdout);
    return 0;
}
