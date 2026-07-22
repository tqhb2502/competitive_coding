// Nearest Shops - CSES 3303
// https://cses.fi/problemset/task/3303
//
// BFS đa nguồn lưu top-2 nhãn (label) khác nhau tại mỗi đỉnh.
// Mỗi thành phố có shop là một nguồn BFS mang nhãn = id của nó.
// Với đỉnh có shop đáp án = d2 (shop khác gần nhất), không shop = d1.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m, k;
    if (scanf("%d %d %d", &n, &m, &k) != 3) return 0;

    vector<char> isShop(n + 1, 0);
    vector<int> shops(k);
    for (int i = 0; i < k; i++) {
        scanf("%d", &shops[i]);
        isShop[shops[i]] = 1;
    }

    // Đọc cạnh, đếm bậc để dựng CSR adjacency.
    vector<int> ea(m), eb(m);
    vector<int> deg(n + 2, 0);
    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        ea[i] = a;
        eb[i] = b;
        deg[a]++;
        deg[b]++;
    }

    vector<int> head(n + 2, 0);
    for (int i = 1; i <= n; i++) head[i + 1] = head[i] + deg[i];
    vector<int> adj(head[n + 1]);
    vector<int> pos(head.begin(), head.end()); // vị trí chèn hiện tại cho mỗi đỉnh
    for (int i = 0; i < m; i++) {
        int a = ea[i], b = eb[i];
        adj[pos[a]++] = b;
        adj[pos[b]++] = a;
    }

    // d1/s1: nguồn gần nhất; d2/s2: nguồn gần nhì có nhãn khác s1.
    const int UNSET = -1;
    vector<int> d1(n + 1, UNSET), s1(n + 1, UNSET);
    vector<int> d2(n + 1, UNSET), s2(n + 1, UNSET);

    struct St { int node, dist, label; };
    queue<St> q;
    for (int i = 0; i < k; i++) {
        int s = shops[i];
        q.push({s, 0, s});
    }

    while (!q.empty()) {
        St cur = q.front();
        q.pop();
        int v = cur.node, dist = cur.dist, label = cur.label;

        if (d1[v] == UNSET) {
            d1[v] = dist;
            s1[v] = label;
        } else if (d2[v] == UNSET && label != s1[v]) {
            d2[v] = dist;
            s2[v] = label;
        } else {
            continue; // nhãn này không nằm trong top-2 của v -> không lan tiếp
        }

        for (int idx = head[v]; idx < head[v + 1]; idx++) {
            int u = adj[idx];
            q.push({u, dist + 1, label});
        }
    }

    // Xuất kết quả.
    string out;
    out.reserve(n * 3);
    char buf[16];
    for (int v = 1; v <= n; v++) {
        int ans = isShop[v] ? d2[v] : d1[v];
        int len = sprintf(buf, "%d", ans);
        out.append(buf, len);
        out.push_back(v == n ? '\n' : ' ');
    }
    fputs(out.c_str(), stdout);
    return 0;
}
