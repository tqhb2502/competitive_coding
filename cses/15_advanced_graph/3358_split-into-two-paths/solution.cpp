#include <bits/stdc++.h>
using namespace std;

// ---------- Đọc nhanh ----------
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

// ---------- Ghi nhanh ----------
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
vector<int> off, to;                 // Danh sách kề dạng CSR (các cạnh ra của DAG)
// matchL[u]: đỉnh kế sau u trong đường đi; matchR[v]: đỉnh đứng trước v
vector<int> matchL, matchR, dist_, ptr_, Q, stk, dv;
const int INF = INT_MAX;

// Phân tầng BFS của Hopcroft-Karp; trả về true nếu còn tồn tại augmenting path.
static bool bfs() {
    int qh = 0, qt = 0;
    for (int u = 1; u <= n; u++) {
        if (matchL[u] == 0) { dist_[u] = 0; Q[qt++] = u; }   // các đỉnh trái tự do
        else dist_[u] = INF;
    }
    bool found = false;
    while (qh < qt) {
        int u = Q[qh++];
        for (int e = off[u]; e < off[u + 1]; e++) {
            int v = to[e];
            int w = matchR[v];
            if (w == 0) found = true;                        // chạm đỉnh phải tự do
            else if (dist_[w] == INF) { dist_[w] = dist_[u] + 1; Q[qt++] = w; }
        }
    }
    return found;
}

// DFS tìm augmenting path từ đỉnh trái tự do su, dùng stack tường minh (iterative).
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
                // Gặp đỉnh phải tự do -> tăng cặp ghép dọc theo cả stack.
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
                dv[u] = v;          // u đi xuống qua đỉnh phải v
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

    // Dựng CSR cho các cạnh ra của DAG (mỗi cạnh u -> v thành u_out - v_in).
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

    // Chạy Hopcroft-Karp nhưng dừng sớm: chỉ cần matching >= n - 2.
    int matched = 0;
    int target = n - 2;
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

    int deficiency = n - matched;   // số đường đi trong phủ = n - M
    if (deficiency > 2) {
        // Cần > 2 đường -> không thể phân hoạch thành đúng hai đường.
        wc('N'); wc('O'); wc('\n');
        fwrite(obuf, 1, opos, stdout);
        return 0;
    }

    // Dựng lại các đường (có đúng `deficiency` đường: 1 hoặc 2).
    wc('Y'); wc('E'); wc('S'); wc('\n');
    vector<vector<int>> paths;
    for (int v = 1; v <= n; v++) {
        if (matchR[v] == 0) {                 // v là điểm bắt đầu một đường
            vector<int> p;
            int c = v;
            // Đi theo matchL để lấy trọn một đường.
            while (c != 0) { p.push_back(c); c = matchL[c]; }
            paths.push_back(move(p));
        }
    }

    // In đúng hai dòng; nếu chỉ có một đường thì đệm thêm một đường rỗng "0".
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
