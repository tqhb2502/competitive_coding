// Forbidden Cities - CSES 1705
// https://cses.fi/problemset/task/1705
//
// Ý tưởng: dùng block-cut tree (block-vertex tree). c tách a và b khi và chỉ
// khi đỉnh thành phố c nằm trên đường đi a-b trong block-vertex tree.
// Kiểm tra "c nằm trên đường a-b" bằng LCA (binary lifting) + đẳng thức
// khoảng cách: dist(a,c)+dist(c,b)==dist(a,b).

#include <bits/stdc++.h>
using namespace std;

// ---------- Fast input ----------
namespace IO {
    const int SZ = 1 << 16;
    char buf[SZ];
    int idx = 0, len = 0;
    inline int gc() {
        if (idx == len) { len = (int)fread(buf, 1, SZ, stdin); idx = 0; if (!len) return -1; }
        return buf[idx++];
    }
    inline int readInt() {
        int c = gc();
        while (c != '-' && (c < '0' || c > '9')) c = gc();
        int s = 1;
        if (c == '-') { s = -1; c = gc(); }
        int x = 0;
        while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
        return x * s;
    }
}
using IO::readInt;

int main() {
    int n = readInt(), m = readInt(), q = readInt();

    vector<vector<pair<int,int>>> adj(n + 1);   // adj[u] = { (v, edgeId) }
    vector<int> eu(m), ev(m);                    // endpoints of each edge
    for (int i = 0; i < m; i++) {
        int a = readInt(), b = readInt();
        eu[i] = a; ev[i] = b;
        adj[a].push_back({b, i});
        adj[b].push_back({a, i});
    }

    // ---------- Biconnected components (Tarjan, edge stack, iterative) ----------
    vector<int> disc(n + 1, -1), low(n + 1, 0);
    int timer = 0;
    vector<int> edgeStack;
    edgeStack.reserve(2 * m + 5);

    // Block-vertex tree: nodes 1..n are cities, blocks get ids n+1, n+2, ...
    vector<vector<int>> tadj(n + 1);   // tadj[node] = neighbours in tree
    int treeNodes = n;                 // next block id will be ++treeNodes
    vector<int> mark(n + 1, 0);        // dedupe vertices inside one block

    struct Frame { int u, pe, idx; };
    vector<Frame> st;
    st.reserve(n + 5);

    for (int s = 1; s <= n; s++) {
        if (disc[s] != -1) continue;
        disc[s] = low[s] = timer++;
        st.push_back({s, -1, 0});
        while (!st.empty()) {
            Frame &fr = st.back();
            int u = fr.u;
            if (fr.idx < (int)adj[u].size()) {
                auto pr = adj[u][fr.idx++];
                int v = pr.first, eid = pr.second;
                if (eid == fr.pe) continue;          // skip the edge we entered by
                if (disc[v] == -1) {                 // tree edge
                    edgeStack.push_back(eid);
                    disc[v] = low[v] = timer++;
                    st.push_back({v, eid, 0});
                } else if (disc[v] < disc[u]) {       // back edge to an ancestor
                    edgeStack.push_back(eid);
                    if (disc[v] < low[u]) low[u] = disc[v];
                }
            } else {
                int pe = fr.pe;
                st.pop_back();
                if (!st.empty()) {
                    int p = st.back().u;
                    if (low[u] < low[p]) low[p] = low[u];
                    if (low[u] >= disc[p]) {          // p is articulation / root -> new block
                        int block = ++treeNodes;
                        tadj.push_back({});
                        while (true) {
                            int e = edgeStack.back(); edgeStack.pop_back();
                            int x = eu[e], y = ev[e];
                            if (mark[x] != block) { mark[x] = block; tadj[block].push_back(x); tadj[x].push_back(block); }
                            if (mark[y] != block) { mark[y] = block; tadj[block].push_back(y); tadj[y].push_back(block); }
                            if (e == pe) break;
                        }
                    }
                }
            }
        }
    }

    // ---------- LCA (binary lifting) over the block-vertex tree ----------
    int V = treeNodes;
    vector<int> depth(V + 1, 0), par(V + 1, 0);
    vector<char> vis(V + 1, 0);

    // BFS from root 1 (graph is connected => tree is connected)
    {
        vector<int> Q; Q.reserve(V);
        Q.push_back(1); vis[1] = 1; par[1] = 0; depth[1] = 0;
        for (size_t h = 0; h < Q.size(); h++) {
            int x = Q[h];
            for (int y : tadj[x]) if (!vis[y]) {
                vis[y] = 1; depth[y] = depth[x] + 1; par[y] = x; Q.push_back(y);
            }
        }
    }

    int LOG = 1;
    while ((1 << LOG) < V) LOG++;
    LOG++;
    vector<vector<int>> up(LOG, vector<int>(V + 1, 0));
    up[0] = par;                       // up[0][1] = 0 (root's parent = sentinel 0)
    for (int k = 1; k < LOG; k++)
        for (int v = 1; v <= V; v++)
            up[k][v] = up[k - 1][ up[k - 1][v] ];

    auto lca = [&](int u, int v) -> int {
        if (depth[u] < depth[v]) swap(u, v);
        int d = depth[u] - depth[v];
        for (int k = 0; k < LOG; k++) if ((d >> k) & 1) u = up[k][u];
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; k--)
            if (up[k][u] != up[k][v]) { u = up[k][u]; v = up[k][v]; }
        return up[0][u];
    };
    auto dist = [&](int u, int v) -> int {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    };

    // ---------- Answer queries ----------
    string out;
    out.reserve(q * 4);
    for (int i = 0; i < q; i++) {
        int a = readInt(), b = readInt(), c = readInt();
        // c nằm trên đường a-b  <=>  dist(a,c)+dist(c,b)==dist(a,b)  => NO
        if (dist(a, c) + dist(c, b) == dist(a, b)) out += "NO\n";
        else out += "YES\n";
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
