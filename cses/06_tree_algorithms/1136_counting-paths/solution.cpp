#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

class DisjointSet {
public:
    explicit DisjointSet(int n) : parent_(n + 1), size_(n + 1, 1) {
        for (int node = 0; node <= n; ++node) {
            parent_[node] = node;
        }
    }

    int find(int node) {
        int root = node;
        while (parent_[root] != root) {
            root = parent_[root];
        }
        while (parent_[node] != node) {
            int next = parent_[node];
            parent_[node] = root;
            node = next;
        }
        return root;
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return;
        }
        if (size_[a] < size_[b]) {
            swap(a, b);
        }
        parent_[b] = a;
        size_[a] += size_[b];
    }

private:
    vector<int> parent_;
    vector<int> size_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, path_count;
    cin >> n >> path_count;

    vector<vector<int>> graph(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    vector<int> parent(n + 1, -1), order;
    vector<vector<int>> children(n + 1);
    order.reserve(n);
    parent[1] = 0;
    order.push_back(1);
    for (int index = 0; index < n; ++index) {
        int node = order[index];
        for (int next : graph[node]) {
            if (next == parent[node]) {
                continue;
            }
            parent[next] = node;
            children[node].push_back(next);
            order.push_back(next);
        }
    }

    vector<int> first(path_count), second(path_count), lca(path_count);
    vector<vector<pair<int, int>>> queries(n + 1);
    for (int id = 0; id < path_count; ++id) {
        cin >> first[id] >> second[id];
        queries[first[id]].push_back({second[id], id});
        queries[second[id]].push_back({first[id], id});
    }

    // Iterative version of Tarjan's offline LCA algorithm.
    DisjointSet dsu(n);
    vector<int> ancestor(n + 1), child_index(n + 1);
    vector<char> finished(n + 1, false);
    vector<int> stack = {1};
    ancestor[1] = 1;

    while (!stack.empty()) {
        int node = stack.back();
        if (child_index[node] < static_cast<int>(children[node].size())) {
            int child = children[node][child_index[node]++];
            ancestor[child] = child;
            stack.push_back(child);
            continue;
        }

        finished[node] = true;
        for (auto [other, id] : queries[node]) {
            if (finished[other]) {
                lca[id] = ancestor[dsu.find(other)];
            }
        }
        stack.pop_back();
        if (parent[node] != 0) {
            int p = parent[node];
            dsu.unite(p, node);
            ancestor[dsu.find(p)] = p;
        }
    }

    vector<long long> difference(n + 1);
    for (int id = 0; id < path_count; ++id) {
        int a = first[id];
        int b = second[id];
        int common = lca[id];
        ++difference[a];
        ++difference[b];
        --difference[common];
        if (parent[common] != 0) {
            --difference[parent[common]];
        }
    }

    for (int index = n - 1; index > 0; --index) {
        int node = order[index];
        difference[parent[node]] += difference[node];
    }
    for (int node = 1; node <= n; ++node) {
        if (node > 1) {
            cout << ' ';
        }
        cout << difference[node];
    }
    cout << '\n';
}
