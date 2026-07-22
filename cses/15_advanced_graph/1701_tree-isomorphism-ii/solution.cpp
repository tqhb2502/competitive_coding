// Tree Isomorphism II - CSES 1701
// https://cses.fi/problemset/task/1701
//
// Đẳng cấu cây không gốc = tìm center(s) rồi so canonical hash (AHU).
// - Mỗi cây có 1 hoặc 2 center (bóc lá theo lớp).
// - AHU: nhãn của 1 đỉnh = id của danh sách nhãn con đã SORT (map dùng chung).
// - So số center; nếu bằng thì root cây 1 tại center, thử từng center cây 2.

#include <bits/stdc++.h>
using namespace std;

static map<vector<int>, int> idMap; // canonical: sorted child-labels -> integer id
static int nextId = 0;

static inline int getId(vector<int>& v) {
    auto it = idMap.find(v);
    if (it != idMap.end()) return it->second;
    int id = nextId++;
    idMap.emplace(v, id);
    return id;
}

// Tìm center(s) của cây bằng cách bóc lá theo từng lớp (topological trimming).
static vector<int> findCenters(int n, const vector<vector<int>>& adj) {
    if (n == 1) return {1};
    vector<int> deg(n + 1);
    vector<int> cur;
    cur.reserve(n);
    for (int i = 1; i <= n; i++) {
        deg[i] = (int)adj[i].size();
        if (deg[i] <= 1) cur.push_back(i);
    }
    int remaining = n;
    while (remaining > 2) {
        vector<int> nxt;
        for (int u : cur) {
            remaining--;
            for (int v : adj[u]) {
                if (--deg[v] == 1) nxt.push_back(v);
            }
        }
        cur.swap(nxt);
    }
    return cur;
}

// Tính canonical label của cây khi root tại `root` (AHU, iterative).
static int rootedLabel(int n, const vector<vector<int>>& adj, int root) {
    vector<int> order;
    order.reserve(n);
    vector<int> parent(n + 1, 0);
    vector<int> label(n + 1, 0);
    vector<char> visited(n + 1, 0);

    order.push_back(root);
    visited[root] = 1;
    parent[root] = 0; // 0 không phải đỉnh hợp lệ (đỉnh đánh số 1..n)
    for (size_t i = 0; i < order.size(); i++) {
        int u = order[i];
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = 1;
                parent[v] = u;
                order.push_back(v);
            }
        }
    }
    // Duyệt ngược thứ tự BFS = post-order hợp lệ (con luôn sau cha).
    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int u = order[i];
        vector<int> childLabels;
        for (int v : adj[u]) {
            if (v != parent[u]) childLabels.push_back(label[v]);
        }
        sort(childLabels.begin(), childLabels.end());
        label[u] = getId(childLabels);
    }
    return label[root];
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int n;
        scanf("%d", &n);
        vector<vector<int>> adj1(n + 1), adj2(n + 1);
        for (int i = 0; i < n - 1; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            adj1[a].push_back(b);
            adj1[b].push_back(a);
        }
        for (int i = 0; i < n - 1; i++) {
            int a, b;
            scanf("%d %d", &a, &b);
            adj2[a].push_back(b);
            adj2[b].push_back(a);
        }

        vector<int> c1 = findCenters(n, adj1);
        vector<int> c2 = findCenters(n, adj2);

        bool ans = false;
        if (c1.size() == c2.size()) {
            int h1 = rootedLabel(n, adj1, c1[0]);
            for (int c : c2) {
                if (rootedLabel(n, adj2, c) == h1) { ans = true; break; }
            }
        }
        printf(ans ? "YES\n" : "NO\n");
    }
    return 0;
}
