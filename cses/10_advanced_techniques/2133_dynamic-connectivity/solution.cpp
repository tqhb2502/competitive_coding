// Dynamic Connectivity - CSES task 2133
// https://cses.fi/problemset/task/2133
//
// Offline dynamic connectivity: segment tree on the time axis (states 0..k)
// + DSU with union-by-size and rollback (no path compression).
// Each edge is alive during one or more contiguous intervals of states.
// We push each (edge, interval) into the O(log) segment-tree nodes covering
// that interval, then DFS the segment tree: at every node union all its edges,
// recurse, and rollback the unions on the way back up. At each leaf (state)
// the number of connected components equals n - (#successful unions on path).

#include <bits/stdc++.h>
using namespace std;

struct RollbackDSU {
    vector<int> par, sz;
    int comp;                       // current number of components
    vector<pair<int,int>> ops;      // (root, child) pairs of real unions, for rollback

    void init(int n) {
        par.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(par.begin(), par.end(), 0);
        comp = n;
        ops.clear();
    }
    int find(int x) {               // no path compression -> rollback friendly
        while (par[x] != x) x = par[x];
        return x;
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;         // no structural change
        if (sz[a] < sz[b]) swap(a, b);
        par[b] = a;
        sz[a] += sz[b];
        --comp;
        ops.emplace_back(a, b);
    }
    void rollback(size_t checkpoint) {
        while (ops.size() > checkpoint) {
            auto [a, b] = ops.back();
            ops.pop_back();
            sz[a] -= sz[b];
            par[b] = b;
            ++comp;
        }
    }
};

int K;                                          // number of events (states are 0..K)
vector<vector<pair<int,int>>> seg;              // edges stored at each segment-tree node
RollbackDSU dsu;
vector<int> ans;

// add edge e to all nodes covering [l, r] within segment covering [nl, nr]
void addEdge(int node, int nl, int nr, int l, int r, const pair<int,int>& e) {
    if (r < nl || nr < l) return;
    if (l <= nl && nr <= r) { seg[node].push_back(e); return; }
    int mid = (nl + nr) / 2;
    addEdge(2 * node,     nl,      mid, l, r, e);
    addEdge(2 * node + 1, mid + 1, nr,  l, r, e);
}

void dfs(int node, int nl, int nr) {
    size_t cp = dsu.ops.size();
    for (const auto& e : seg[node]) dsu.unite(e.first, e.second);
    if (nl == nr) {
        ans[nl] = dsu.comp;
    } else {
        int mid = (nl + nr) / 2;
        dfs(2 * node,     nl,      mid);
        dfs(2 * node + 1, mid + 1, nr);
    }
    dsu.rollback(cp);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m >> K)) return 0;

    const long long BASE = 100001LL;            // n <= 1e5, safe encoding of a pair
    auto enc = [&](int a, int b) -> long long {
        if (a > b) swap(a, b);
        return (long long)a * BASE + b;
    };

    // active[key] = stack of start-states of currently present copies of that edge
    unordered_map<long long, vector<int>> active;
    active.reserve(2 * (m + K) + 16);

    for (int i = 0; i < m; ++i) {
        int a, b; cin >> a >> b;
        active[enc(a, b)].push_back(0);         // initial edges exist from state 0
    }

    // segment tree over states [0, K] -> K+1 leaves
    seg.assign(4 * (K + 1), {});
    ans.assign(K + 1, 0);

    for (int i = 1; i <= K; ++i) {
        int t, a, b; cin >> t >> a >> b;
        long long key = enc(a, b);
        if (t == 1) {
            active[key].push_back(i);           // present starting at state i
        } else {
            // remove: edge was present over [start, i-1]
            auto& st = active[key];
            int start = st.back();
            st.pop_back();
            addEdge(1, 0, K, start, i - 1, make_pair(a, b));
        }
    }

    // edges that are never removed live until the last state K
    for (auto& kv : active) {
        if (kv.second.empty()) continue;
        int a = (int)(kv.first / BASE);
        int b = (int)(kv.first % BASE);
        for (int start : kv.second) {
            addEdge(1, 0, K, start, K, make_pair(a, b));
        }
    }

    dsu.init(n);
    dfs(1, 0, K);

    // print K+1 component counts
    string out;
    out.reserve((size_t)(K + 1) * 7);
    for (int i = 0; i <= K; ++i) {
        out += to_string(ans[i]);
        out += (i == K ? '\n' : ' ');
    }
    fputs(out.c_str(), stdout);
    return 0;
}
