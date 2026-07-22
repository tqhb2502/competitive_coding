// Split into Two Paths - CSES 3358
// https://cses.fi/problemset/task/3358
//
// Phân hoạch các đỉnh của một DAG thành <= 2 đường đi rời đỉnh (một đường có thể
// rỗng). Đây là bài toán Minimum Path Cover: số đường tối thiểu = n - M, với M là
// maximum bipartite matching (u_out - v_in cho mỗi cạnh u->v). Đáp án YES <=> M >= n-2.
// Sử dụng Hopcroft-Karp; DFS augmenting dùng stack tường minh (iterative).

#include <bits/stdc++.h>
using namespace std;

// ---------- fast input ----------
static char ibuf[1 << 16];
static int ilen = 0, ipos = 0;
static inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen <= 0) return -1;
    }
    return ibuf[ipos++];
}
static inline int readInt() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    if (c == -1) return -1;
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return x;
}

// ---------- fast output ----------
static char obuf[1 << 23];
static int opos = 0;
static inline void wc(char c) { obuf[opos++] = c; }
static inline void wint(int x) {
    if (x == 0) { obuf[opos++] = '0'; return; }
    char tmp[12]; int t = 0;
    while (x) { tmp[t++] = char('0' + x % 10); x /= 10; }
    while (t--) obuf[opos++] = tmp[t];
}

int n, m;
vector<int> off, to;                 // CSR adjacency (out-edges of the DAG)
vector<int> matchL, matchR, dist_, ptr_, Q, stk, dv;
const int INF = INT_MAX;

// BFS layering of Hopcroft-Karp; returns true if an augmenting path exists.
static bool bfs() {
    int qh = 0, qt = 0;
    for (int u = 1; u <= n; u++) {
        if (matchL[u] == 0) { dist_[u] = 0; Q[qt++] = u; }
        else dist_[u] = INF;
    }
    bool found = false;
    while (qh < qt) {
        int u = Q[qh++];
        for (int e = off[u]; e < off[u + 1]; e++) {
            int v = to[e];
            int w = matchR[v];
            if (w == 0) found = true;
            else if (dist_[w] == INF) { dist_[w] = dist_[u] + 1; Q[qt++] = w; }
        }
    }
    return found;
}

// Iterative augmenting DFS from a free left vertex su.
static bool augment(int su) {
    int sp = 0;
    stk[sp++] = su;
    while (sp > 0) {
        int u = stk[sp - 1];
        bool descended = false;
        while (ptr_[u] < off[u + 1]) {
            int v = to[ptr_[u]++];
            int w = matchR[v];
            if (w == 0) {
                // Reached a free right vertex -> augment along the whole stack.
                matchR[v] = u; matchL[u] = v;
                sp--;
                while (sp > 0) {
                    int p = stk[sp - 1];
                    int pv = dv[p];
                    matchR[pv] = p; matchL[p] = pv;
                    sp--;
                }
                return true;
            } else if (dist_[w] == dist_[u] + 1) {
                dv[u] = v;          // u descends via right vertex v
                stk[sp++] = w;
                descended = true;
                break;
            }
        }
        if (!descended) { dist_[u] = INF; sp--; }
    }
    return false;
}

int main() {
    n = readInt();
    m = readInt();

    vector<int> ea(m), eb(m);
    off.assign(n + 2, 0);
    for (int i = 0; i < m; i++) {
        int a = readInt(), b = readInt();
        ea[i] = a; eb[i] = b;
        off[a + 1]++;
    }
    for (int i = 1; i <= n + 1; i++) off[i] += off[i - 1];
    to.assign(max(m, 1), 0);
    {
        vector<int> fillp(off.begin(), off.end());
        for (int i = 0; i < m; i++) {
            int a = ea[i];
            to[fillp[a]++] = eb[i];
        }
    }

    matchL.assign(n + 1, 0);
    matchR.assign(n + 1, 0);
    dist_.assign(n + 1, 0);
    ptr_.assign(n + 1, 0);
    Q.assign(n + 1, 0);
    stk.assign(n + 1, 0);
    dv.assign(n + 1, 0);

    int matched = 0;
    int target = n - 2;             // we only need matching >= n-2
    while (matched < target) {
        if (!bfs()) break;
        for (int u = 1; u <= n; u++) ptr_[u] = off[u];
        for (int u = 1; u <= n; u++) {
            if (matchL[u] == 0) {
                if (augment(u)) {
                    matched++;
                    if (matched >= target) break;
                }
            }
        }
    }

    int deficiency = n - matched;   // number of paths in the cover
    if (deficiency > 2) {
        wc('N'); wc('O'); wc('\n');
        fwrite(obuf, 1, opos, stdout);
        return 0;
    }

    // Reconstruct the paths (there are exactly `deficiency` of them: 1 or 2).
    wc('Y'); wc('E'); wc('S'); wc('\n');
    vector<vector<int>> paths;
    for (int v = 1; v <= n; v++) {
        if (matchR[v] == 0) {                 // v is a path start
            vector<int> p;
            int c = v;
            while (c != 0) { p.push_back(c); c = matchL[c]; }
            paths.push_back(move(p));
        }
    }

    // Print exactly two lines; pad with an empty path if there is only one.
    for (int idx = 0; idx < 2; idx++) {
        if (idx < (int)paths.size()) {
            const vector<int>& p = paths[idx];
            wint((int)p.size());
            for (int x : p) { wc(' '); wint(x); }
            wc('\n');
        } else {
            wint(0);
            wc('\n');
        }
    }

    fwrite(obuf, 1, opos, stdout);
    return 0;
}
