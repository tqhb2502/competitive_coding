// Reversals and Sums - CSES 2074
// https://cses.fi/problemset/task/2074
//
// Implicit treap (Cartesian tree) voi lazy reversal.
// Moi node luu: con trai/phai, priority, gia tri, kich thuoc subtree,
// tong subtree (long long) va mot lazy flag rev.
// split(theo so phan tu) + merge chay O(log n) ky vong.
// Reverse [a,b]: split ra ba phan, applyRev cho phan giua, merge lai.
// Sum [a,b]: split ra ba phan, doc sm cua phan giua, merge lai.

#include <bits/stdc++.h>
using namespace std;

// -------- Fast IO --------
static char ibuf[1 << 25];
static int ipos = 0, ilen = 0;

inline int gc() {
    if (ipos == ilen) {
        ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin);
        ipos = 0;
        if (ilen == 0) return -1;
    }
    return ibuf[ipos++];
}

inline long long readLL() {
    int c = gc();
    while (c != '-' && (c < '0' || c > '9')) c = gc();
    bool neg = false;
    if (c == '-') { neg = true; c = gc(); }
    long long x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return neg ? -x : x;
}

static char obuf[1 << 22];
static int opos = 0;

inline void flushOut() {
    fwrite(obuf, 1, opos, stdout);
    opos = 0;
}

inline void writeLL(long long x) {
    if (opos > (int)sizeof(obuf) - 32) flushOut();
    if (x < 0) { obuf[opos++] = '-'; x = -x; }
    char tmp[24];
    int len = 0;
    if (x == 0) tmp[len++] = '0';
    while (x) { tmp[len++] = char('0' + x % 10); x /= 10; }
    while (len) obuf[opos++] = tmp[--len];
    obuf[opos++] = '\n';
}

const int MAXN = 200005;

int lc[MAXN], rc[MAXN], sz[MAXN];
unsigned int prio[MAXN];
long long val[MAXN], sm[MAXN];
bool rev_[MAXN];
int tot = 0;
int root = 0;

static mt19937 rng(
    (unsigned int)chrono::steady_clock::now().time_since_epoch().count());

inline int newNode(long long v) {
    int x = ++tot;
    lc[x] = rc[x] = 0;
    sz[x] = 1;
    prio[x] = rng();
    val[x] = v;
    sm[x] = v;
    rev_[x] = false;
    return x;
}

inline void pull(int x) {
    // node 0 co sz=0, sm=0 (cac mang global khoi tao ve 0)
    sz[x] = sz[lc[x]] + sz[rc[x]] + 1;
    sm[x] = sm[lc[x]] + sm[rc[x]] + val[x];
}

inline void applyRev(int x) {
    if (!x) return;
    swap(lc[x], rc[x]);
    rev_[x] ^= 1;
}

inline void pushDown(int x) {
    if (rev_[x]) {
        applyRev(lc[x]);
        applyRev(rc[x]);
        rev_[x] = false;
    }
}

// Tach x thanh (l, r): l gom k phan tu dau tien, r gom phan con lai.
void split(int x, int k, int &l, int &r) {
    if (!x) {
        l = r = 0;
        return;
    }
    pushDown(x);
    if (sz[lc[x]] + 1 <= k) {
        l = x;
        split(rc[x], k - sz[lc[x]] - 1, rc[x], r);
        pull(x);
    } else {
        r = x;
        split(lc[x], k, l, lc[x]);
        pull(x);
    }
}

// Ghep hai treap: a dung truoc, b dung sau.
int merge(int a, int b) {
    if (!a) return b;
    if (!b) return a;
    if (prio[a] > prio[b]) {
        pushDown(a);
        rc[a] = merge(rc[a], b);
        pull(a);
        return a;
    } else {
        pushDown(b);
        lc[b] = merge(a, lc[b]);
        pull(b);
        return b;
    }
}

int main() {
    int n = (int)readLL();
    int m = (int)readLL();

    for (int i = 0; i < n; i++) {
        long long v = readLL();
        root = merge(root, newNode(v));
    }

    while (m--) {
        int t = (int)readLL();
        int a = (int)readLL();
        int b = (int)readLL();
        int L, M, R;
        // L = [1, a-1], M = [a, b], R = [b+1, n]
        split(root, a - 1, L, M);
        split(M, b - a + 1, M, R);
        if (t == 1) {
            applyRev(M);
        } else {
            writeLL(sm[M]);
        }
        root = merge(merge(L, M), R);
    }
    flushOut();
    return 0;
}
