// Necessary Cities - CSES 2077
// https://cses.fi/problemset/task/2077
// Tim tat ca articulation points (cut vertices) bang thuat toan Tarjan.
// Dung iterative DFS de tranh tran stack khi n toi 1e5.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    vector<int> disc(n + 1, 0), low(n + 1, 0), parent(n + 1, -1);
    vector<int> it(n + 1, 0), childCount(n + 1, 0);
    vector<char> isAP(n + 1, 0);
    int timer = 0;

    vector<int> st;
    st.reserve(n);

    for (int s = 1; s <= n; s++) {
        if (disc[s]) continue;
        parent[s] = -1;
        disc[s] = low[s] = ++timer;
        st.push_back(s);

        while (!st.empty()) {
            int u = st.back();
            if (it[u] < (int)adj[u].size()) {
                int v = adj[u][it[u]++];
                if (!disc[v]) {
                    parent[v] = u;
                    childCount[u]++;
                    disc[v] = low[v] = ++timer;
                    st.push_back(v);
                } else if (v != parent[u]) {
                    // back edge: cap nhat low bang disc cua to tien
                    low[u] = min(low[u], disc[v]);
                }
            } else {
                // duyet xong u -> pop va cap nhat cho cha
                st.pop_back();
                int p = parent[u];
                if (p != -1) {
                    low[p] = min(low[p], low[u]);
                    if (parent[p] != -1 && low[u] >= disc[p]) {
                        isAP[p] = 1;
                    }
                }
            }
        }

        // truong hop goc: >= 2 con trong cay DFS thi goc la dinh cat
        if (childCount[s] >= 2) isAP[s] = 1;
    }

    vector<int> res;
    for (int i = 1; i <= n; i++) {
        if (isAP[i]) res.push_back(i);
    }

    cout << res.size() << "\n";
    for (size_t i = 0; i < res.size(); i++) {
        cout << res[i];
        if (i + 1 < res.size()) cout << ' ';
    }
    cout << "\n";

    return 0;
}
