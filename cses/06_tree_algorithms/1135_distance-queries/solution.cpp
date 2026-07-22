#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<vector<int>> graph(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    vector<int> parent(n + 1), depth(n + 1);
    queue<int> bfs;
    bfs.push(1);
    parent[1] = 0;
    while (!bfs.empty()) {
        int node = bfs.front();
        bfs.pop();
        for (int next : graph[node]) {
            if (next == parent[node]) {
                continue;
            }
            parent[next] = node;
            depth[next] = depth[node] + 1;
            bfs.push(next);
        }
    }

    int levels = 1;
    while ((1LL << levels) <= n) {
        ++levels;
    }
    vector<vector<int>> up(levels, vector<int>(n + 1));
    up[0] = parent;
    for (int level = 1; level < levels; ++level) {
        for (int node = 1; node <= n; ++node) {
            up[level][node] = up[level - 1][up[level - 1][node]];
        }
    }

    auto lca = [&](int a, int b) {
        if (depth[a] < depth[b]) {
            swap(a, b);
        }
        int difference = depth[a] - depth[b];
        for (int level = 0; level < levels; ++level) {
            if ((difference >> level) & 1) {
                a = up[level][a];
            }
        }
        if (a == b) {
            return a;
        }
        for (int level = levels - 1; level >= 0; --level) {
            if (up[level][a] != up[level][b]) {
                a = up[level][a];
                b = up[level][b];
            }
        }
        return parent[a];
    };

    while (q--) {
        int a, b;
        cin >> a >> b;
        int ancestor = lca(a, b);
        cout << depth[a] + depth[b] - 2 * depth[ancestor] << '\n';
    }
}
