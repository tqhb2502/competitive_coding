#include <bits/stdc++.h>
using namespace std;

// ---------- Đọc số nhanh ----------
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
    vector<int> eu(m), ev(m);                    // hai đầu mút của mỗi cạnh
    for (int i = 0; i < m; i++) {
        int a = readInt(), b = readInt();
        eu[i] = a; ev[i] = b;
        adj[a].push_back({b, i});
        adj[b].push_back({a, i});
    }

    // ---------- Phân rã biconnected component (Tarjan, stack cạnh, khử đệ quy) ----------
    vector<int> disc(n + 1, -1), low(n + 1, 0);
    int timer = 0;
    vector<int> edgeStack;                       // stack các cạnh của BCC đang xét
    edgeStack.reserve(2 * m + 5);

    // Block-vertex tree: đỉnh 1..n là thành phố, mỗi block (BCC) nhận id n+1, n+2, ...
    vector<vector<int>> tadj(n + 1);   // tadj[node] = danh sách kề trong cây
    int treeNodes = n;                 // id block kế tiếp = ++treeNodes
    vector<int> mark(n + 1, 0);        // khử trùng đỉnh trong cùng một block

    struct Frame { int u, pe, idx; };
    vector<Frame> st;                  // stack tường minh thay cho đệ quy DFS
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
                if (eid == fr.pe) continue;          // bỏ qua cạnh vừa đi vào
                if (disc[v] == -1) {                 // cạnh cây (tree edge)
                    edgeStack.push_back(eid);
                    disc[v] = low[v] = timer++;
                    st.push_back({v, eid, 0});
                } else if (disc[v] < disc[u]) {       // cạnh ngược lên tổ tiên (back edge)
                    edgeStack.push_back(eid);
                    if (disc[v] < low[u]) low[u] = disc[v];
                }
            } else {
                // Đã duyệt hết cạnh của u -> quay lui, cập nhật low cho cha
                int pe = fr.pe;
                st.pop_back();
                if (!st.empty()) {
                    int p = st.back().u;
                    if (low[u] < low[p]) low[p] = low[u];
                    if (low[u] >= disc[p]) {          // p là khớp / gốc -> chốt một block mới
                        int block = ++treeNodes;
                        tadj.push_back({});
                        // Lấy các cạnh trên stack tới cạnh (p, u) làm thành một BCC,
                        // rồi nối mọi đỉnh của BCC đó vào đỉnh block trong cây.
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

    // ---------- LCA (binary lifting) trên block-vertex tree ----------
    int V = treeNodes;
    vector<int> depth(V + 1, 0), par(V + 1, 0);
    vector<char> vis(V + 1, 0);

    // BFS từ gốc 1 (đồ thị liên thông => cây liên thông), tính depth và cha
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
    vector<vector<int>> up(LOG, vector<int>(V + 1, 0));   // up[k][v] = tổ tiên thứ 2^k của v
    up[0] = par;                       // up[0][1] = 0 (cha của gốc = đỉnh ảo 0)
    for (int k = 1; k < LOG; k++)
        for (int v = 1; v <= V; v++)
            up[k][v] = up[k - 1][ up[k - 1][v] ];

    auto lca = [&](int u, int v) -> int {
        if (depth[u] < depth[v]) swap(u, v);
        int d = depth[u] - depth[v];
        for (int k = 0; k < LOG; k++) if ((d >> k) & 1) u = up[k][u];   // kéo u lên cùng độ sâu
        if (u == v) return u;
        for (int k = LOG - 1; k >= 0; k--)
            if (up[k][u] != up[k][v]) { u = up[k][u]; v = up[k][v]; }   // nâng đồng thời tới sát LCA
        return up[0][u];
    };
    // Khoảng cách trên cây: dist(u, v) = depth[u] + depth[v] - 2 * depth[lca(u, v)]
    auto dist = [&](int u, int v) -> int {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    };

    // ---------- Trả lời truy vấn ----------
    string out;
    out.reserve(q * 4);
    for (int i = 0; i < q; i++) {
        int a = readInt(), b = readInt(), c = readInt();
        // c nằm trên đường a-b <=> dist(a,c)+dist(c,b)==dist(a,b) => c tách a, b => "NO"
        if (dist(a, c) + dist(c, b) == dist(a, b)) out += "NO\n";
        else out += "YES\n";
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
