// Reachability Queries - CSES 2143
// https://cses.fi/problemset/task/2143
//
// Huong tiep can:
//  1) Nen do thi co huong bang SCC (Tarjan lap - iterative de tranh de quy sau).
//  2) Tren DAG condensation, tinh transitive closure bang bitset: xu ly cac
//     component theo thu tu id tang dan (Tarjan tra ve reverse-topological),
//     reach[c] = {c} hop voi union cua reach[successor].
//  3) Truy van (a,b): kiem tra bit comp(b) trong reach[comp(a)].
//
// Voi n <= 5*10^4 => so SCC C <= 5*10^4, bitset dung C tu 64-bit / component,
// bo nho ~ C*ceil(C/64)*8 <= ~312MB (nam trong 512MB).

#include <bits/stdc++.h>
using namespace std;

namespace fio {
    const int BS = 1 << 16;
    char buf[BS];
    int len = 0, pos = 0;
    inline int gc() {
        if (pos == len) { len = (int)fread(buf, 1, BS, stdin); pos = 0; if (len <= 0) return -1; }
        return buf[pos++];
    }
    inline int readInt() {
        int c = gc();
        while (c != -1 && (c < '0' || c > '9')) c = gc();
        int x = 0;
        while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
        return x;
    }
}

int main() {
    int n = fio::readInt();
    int m = fio::readInt();
    int q = fio::readInt();

    vector<int> ea(m), eb(m);
    vector<int> deg(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int a = fio::readInt() - 1, b = fio::readInt() - 1;
        ea[i] = a; eb[i] = b; deg[a]++;
    }

    // CSR danh sach ke ra (out-adjacency)
    vector<int> head(n + 1, 0);
    for (int i = 0; i < n; i++) head[i + 1] = head[i] + deg[i];
    vector<int> adj(m);
    {
        vector<int> cur(head.begin(), head.end());
        for (int i = 0; i < m; i++) adj[cur[ea[i]]++] = eb[i];
    }

    // Tarjan SCC (iterative)
    vector<int> disc(n, -1), low(n, 0), comp(n, -1), it(n);
    for (int i = 0; i < n; i++) it[i] = head[i];
    vector<char> onstk(n, 0);
    vector<int> S;  S.reserve(n);   // Tarjan stack
    vector<int> cs; cs.reserve(n);  // DFS call stack (iterative)
    int timer = 0, C = 0;
    for (int s = 0; s < n; s++) {
        if (disc[s] != -1) continue;
        cs.push_back(s);
        disc[s] = low[s] = timer++; onstk[s] = 1; S.push_back(s);
        while (!cs.empty()) {
            int u = cs.back();
            if (it[u] < head[u + 1]) {
                int v = adj[it[u]++];
                if (disc[v] == -1) {
                    disc[v] = low[v] = timer++; onstk[v] = 1; S.push_back(v);
                    cs.push_back(v);
                } else if (onstk[v]) {
                    if (disc[v] < low[u]) low[u] = disc[v];
                }
            } else {
                if (low[u] == disc[u]) {
                    while (true) {
                        int w = S.back(); S.pop_back();
                        onstk[w] = 0; comp[w] = C;
                        if (w == u) break;
                    }
                    C++;
                }
                cs.pop_back();
                if (!cs.empty()) {
                    int p = cs.back();
                    if (low[u] < low[p]) low[p] = low[u];
                }
            }
        }
    }

    // DAG condensation: canh comp(u)->comp(v) chi ton tai khi comp(v) < comp(u)
    // (Tarjan danh so theo reverse-topological order).
    vector<int> cdeg(C, 0);
    for (int i = 0; i < m; i++) {
        int cu = comp[ea[i]], cv = comp[eb[i]];
        if (cu != cv) cdeg[cu]++;
    }
    vector<int> chead(C + 1, 0);
    for (int i = 0; i < C; i++) chead[i + 1] = chead[i] + cdeg[i];
    vector<int> cadj(chead[C]);
    {
        vector<int> cur(chead.begin(), chead.end());
        for (int i = 0; i < m; i++) {
            int cu = comp[ea[i]], cv = comp[eb[i]];
            if (cu != cv) cadj[cur[cu]++] = cv;
        }
    }

    // Transitive closure bang bitset (W tu 64-bit / component).
    // Xu ly id tang dan: moi successor (id nho hon) da tinh xong truoc.
    int W = (C + 63) / 64;
    uint64_t* reach = new uint64_t[(size_t)C * W]();
    for (int c = 0; c < C; c++) {
        uint64_t* rc = reach + (size_t)c * W;
        rc[c >> 6] |= (1ULL << (c & 63));
        for (int e = chead[c]; e < chead[c + 1]; e++) {
            const uint64_t* rd = reach + (size_t)cadj[e] * W;
            for (int j = 0; j < W; j++) rc[j] |= rd[j];
        }
    }

    // Truy van
    string out;
    out.reserve((size_t)q * 4);
    for (int i = 0; i < q; i++) {
        int a = fio::readInt() - 1, b = fio::readInt() - 1;
        int ca = comp[a], cb = comp[b];
        const uint64_t* rc = reach + (size_t)ca * W;
        if ((rc[cb >> 6] >> (cb & 63)) & 1ULL) out += "YES\n";
        else out += "NO\n";
    }
    fwrite(out.data(), 1, out.size(), stdout);

    delete[] reach;
    return 0;
}
