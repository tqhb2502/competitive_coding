#include <bits/stdc++.h>
using namespace std;

// DSU (Union-Find) với path compression + union by rank.
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

    // Đọc từng cạnh (a, b, w), lưu chỉ số gốc để in đúng thứ tự input.
    vector<int> a(m), b(m), w(m), idx(m);
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &a[i], &b[i], &w[i]);
        idx[i] = i;
    }

    // Sắp xếp các cạnh theo trọng số tăng dần.
    sort(idx.begin(), idx.end(), [&](int x, int y) { return w[x] < w[y]; });

    for (int i = 1; i <= n; i++) { par[i] = i; rnk[i] = 0; }

    vector<char> ans(m, 0); // 1 = YES, 0 = NO

    // Duyệt theo từng NHÓM cạnh có cùng trọng số w.
    int i = 0;
    while (i < m) {
        int j = i;
        while (j < m && w[idx[j]] == w[idx[i]]) j++;

        // Kiểm tra cả nhóm [i, j) TRƯỚC khi union:
        // lúc này DSU chỉ chứa các cạnh trọng số < w. Cạnh nằm trong MỘT MST
        // nào đó khi và chỉ khi hai đầu mút còn khác thành phần liên thông.
        for (int k = i; k < j; k++) {
            int e = idx[k];
            ans[e] = (findp(a[e]) != findp(b[e])) ? 1 : 0;
        }

        // SAU khi kiểm tra xong cả nhóm mới union toàn bộ cạnh của nhóm.
        for (int k = i; k < j; k++) {
            int e = idx[k];
            unite(a[e], b[e]);
        }
        i = j;
    }

    // In kết quả theo đúng thứ tự cạnh trong input.
    string out;
    out.reserve((size_t)m * 4);
    for (int k = 0; k < m; k++) out += (ans[k] ? "YES\n" : "NO\n");
    fputs(out.c_str(), stdout);
    return 0;
}
