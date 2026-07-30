#include <bits/stdc++.h>
using namespace std;

// Đọc số nguyên nhanh bằng buffer tự quản lý
static char ibuf[1 << 23];
static int ilen = 0, ipos = 0;
static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen == 0) return -1;
    }
    return ibuf[ipos++];
}
static inline int readInt() {
    int c = gc();
    while (c != '-' && (c < '0' || c > '9')) c = gc();
    int sgn = 1;
    if (c == '-') { sgn = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return x * sgn;
}

const int LOG = 18;      // 2^18 = 262144 > 2e5
const int INF = 1e9;

int N;
// Danh sách kề dạng linked-list (đầu danh sách, con trỏ kế, đỉnh đích)
vector<int> adj_head, adj_nxt, adj_to;
int edgeCnt = 0;
inline void addEdge(int u, int v) {
    adj_to[edgeCnt] = v; adj_nxt[edgeCnt] = adj_head[u]; adj_head[u] = edgeCnt++;
}

int nd[200005];                 // nearestCoin: khoảng cách từ đỉnh tới đồng xu gần nhất
int par[200005], dep[200005];   // cha và độ sâu của mỗi đỉnh
int up[LOG][200005];            // binary lifting: tổ tiên thứ 2^k
int minf[LOG][200005];          // min nearestCoin trên đoạn 2^k đỉnh tính từ v đi lên

int main() {
    int n = readInt(), q = readInt();
    N = n;
    vector<int> coin(n + 1);
    for (int i = 1; i <= n; i++) coin[i] = readInt();

    adj_head.assign(n + 1, -1);
    adj_nxt.assign(2 * (n - 1 > 0 ? n - 1 : 1), 0);
    adj_to.assign(2 * (n - 1 > 0 ? n - 1 : 1), 0);
    for (int i = 0; i < n - 1; i++) {
        int a = readInt(), b = readInt();
        addEdge(a, b);
        addEdge(b, a);
    }

    // Multi-source BFS (BFS đa nguồn): xuất phát đồng thời từ mọi đỉnh có đồng xu
    // để tính nearestCoin cho tất cả các đỉnh trong O(n)
    for (int i = 1; i <= n; i++) nd[i] = INF;
    {
        vector<int> Q;
        Q.reserve(n);
        for (int i = 1; i <= n; i++) if (coin[i]) { nd[i] = 0; Q.push_back(i); }
        for (size_t h = 0; h < Q.size(); h++) {
            int u = Q[h];
            for (int e = adj_head[u]; e != -1; e = adj_nxt[e]) {
                int w = adj_to[e];
                if (nd[w] > nd[u] + 1) { nd[w] = nd[u] + 1; Q.push_back(w); }
            }
        }
    }

    // BFS dựng cha và độ sâu từ gốc 1 (lặp, không đệ quy để tránh tràn stack)
    {
        vector<char> vis(n + 1, 0);
        vector<int> Q;
        Q.reserve(n);
        Q.push_back(1); vis[1] = 1; par[1] = 0; dep[1] = 0;
        for (size_t h = 0; h < Q.size(); h++) {
            int u = Q[h];
            for (int e = adj_head[u]; e != -1; e = adj_nxt[e]) {
                int w = adj_to[e];
                if (!vis[w]) { vis[w] = 1; par[w] = u; dep[w] = dep[u] + 1; Q.push_back(w); }
            }
        }
    }

    // Dựng bảng binary lifting; đỉnh 0 là sentinel: up->0, minf->INF, nd->INF
    nd[0] = INF;
    for (int u = 1; u <= n; u++) { up[0][u] = par[u]; minf[0][u] = nd[u]; }
    up[0][0] = 0; minf[0][0] = INF;
    for (int k = 1; k < LOG; k++) {
        for (int u = 0; u <= n; u++) {
            int mid = up[k - 1][u];
            up[k][u] = up[k - 1][mid];
            minf[k][u] = min(minf[k - 1][u], minf[k - 1][mid]);
        }
    }

    // LCA bằng binary lifting: cân bằng độ sâu rồi cùng leo lên
    auto lca = [&](int a, int b) -> int {
        if (dep[a] < dep[b]) swap(a, b);
        int diff = dep[a] - dep[b];
        for (int k = 0; k < LOG; k++) if (diff & (1 << k)) a = up[k][a];
        if (a == b) return a;
        for (int k = LOG - 1; k >= 0; k--) if (up[k][a] != up[k][b]) { a = up[k][a]; b = up[k][b]; }
        return up[0][a];
    };

    // min nearestCoin trên (steps+1) đỉnh: v, par(v), ..., tổ tiên cách v `steps` bước
    auto climbMin = [&](int v, int steps) -> int {
        int cnt = steps + 1;
        int res = INF;
        for (int k = 0; k < LOG; k++) if (cnt & (1 << k)) {
            res = min(res, minf[k][v]);
            v = up[k][v];
        }
        return res;
    };

    // Trả lời truy vấn: đáp án = dist_tree(a, b) + 2 * min nearestCoin trên path(a, b)
    string out;
    out.reserve((size_t)q * 7);
    char tmp[24];
    for (int i = 0; i < q; i++) {
        int a = readInt(), b = readInt();
        int L = lca(a, b);
        long long dist = (long long)dep[a] + dep[b] - 2LL * dep[L];
        // Tách path a-b thành a->LCA và b->LCA, lấy min nearestCoin trên cả hai nửa
        int m = min(climbMin(a, dep[a] - dep[L]), climbMin(b, dep[b] - dep[L]));
        long long ans = dist + 2LL * m;
        int len = sprintf(tmp, "%lld\n", ans);
        out.append(tmp, len);
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
