#include <bits/stdc++.h>
using namespace std;

namespace FastIO {
    const int SZ = 1 << 20;
    char ibuf[SZ];
    int ipos = 0, ilen = 0;
    inline int gc() {
        if (ipos == ilen) {
            ilen = (int)fread(ibuf, 1, SZ, stdin);
            ipos = 0;
            if (ilen == 0) return -1;
        }
        return ibuf[ipos++];
    }
    inline long long readInt() {
        int c = gc();
        while (c != -1 && (c < '0' || c > '9') && c != '-') c = gc();
        long long s = 1;
        if (c == '-') { s = -1; c = gc(); }
        long long x = 0;
        while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
        return x * s;
    }
}
using FastIO::readInt;

static int par[100005];
static inline int findp(int x) {
    while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; }
    return x;
}

int main() {
    int n = (int)readInt(), m = (int)readInt(), q = (int)readInt();

    vector<int> A(m + 1), B(m + 1), W(m + 1), CU(m + 1), CV(m + 1);
    for (int i = 1; i <= m; i++) {
        A[i] = (int)readInt();
        B[i] = (int)readInt();
        W[i] = (int)readInt();
    }

    // Sắp xếp các chỉ số cạnh theo trọng số tăng dần.
    vector<int> idx(m);
    for (int i = 0; i < m; i++) idx[i] = i + 1;
    sort(idx.begin(), idx.end(), [&](int x, int y) { return W[x] < W[y]; });

    // DSU cho tiền xử lý (mô phỏng Kruskal theo lớp trọng số).
    for (int i = 1; i <= n; i++) par[i] = i;
    int i = 0;
    while (i < m) {
        int j = i;
        while (j < m && W[idx[j]] == W[idx[i]]) j++;
        // Trước khi union nhóm này, DSU = các thành phần của G_{<w}.
        for (int k = i; k < j; k++) {
            int e = idx[k];
            CU[e] = findp(A[e]);
            CV[e] = findp(B[e]);
        }
        // Union toàn bộ cạnh trọng số w để chuyển sang trọng số kế tiếp.
        for (int k = i; k < j; k++) {
            int e = idx[k];
            int ra = findp(A[e]), rb = findp(B[e]);
            if (ra != rb) par[ra] = rb;
        }
        i = j;
    }

    // DSU tạm cho truy vấn: tpar[x]==0 nghĩa là chưa khởi tạo (id đỉnh >= 1).
    vector<int> tpar(n + 1, 0);
    vector<int> touched;
    touched.reserve(1 << 16);

    vector<pair<int, int>> ed; // (weight, edgeIndex)
    string out;
    out.reserve((size_t)q * 4);

    for (int Q = 0; Q < q; Q++) {
        int k = (int)readInt();
        ed.clear();
        ed.reserve(k);
        for (int t = 0; t < k; t++) {
            int e = (int)readInt();
            ed.push_back({W[e], e});
        }
        sort(ed.begin(), ed.end(),
             [](const pair<int, int>& x, const pair<int, int>& y) {
                 return x.first < y.first;
             });

        bool ok = true;
        int p = 0;
        int sz = (int)ed.size();
        while (p < sz && ok) {
            int wcur = ed[p].first;
            int r = p;
            while (r < sz && ed[r].first == wcur) {
                int e = ed[r].second;
                int u = CU[e], v = CV[e];
                if (tpar[u] == 0) { tpar[u] = u; touched.push_back(u); }
                if (tpar[v] == 0) { tpar[v] = v; touched.push_back(v); }
                int ru = u; while (tpar[ru] != ru) { tpar[ru] = tpar[tpar[ru]]; ru = tpar[ru]; }
                int rv = v; while (tpar[rv] != rv) { tpar[rv] = tpar[tpar[rv]]; rv = tpar[rv]; }
                if (ru == rv) { ok = false; break; }
                tpar[ru] = rv;
                r++;
            }
            // Reset DSU tạm sau mỗi nhóm trọng số (các lớp độc lập).
            for (int x : touched) tpar[x] = 0;
            touched.clear();
            p = r;
        }

        out += ok ? "YES\n" : "NO\n";
    }

    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
