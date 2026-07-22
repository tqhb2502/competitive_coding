// Substring Reversals - CSES 2073
// https://cses.fi/problemset/task/2073
//
// Implicit treap (cartesian tree keyed by array position) with a lazy
// "reverse" flag. To reverse the substring [a, b] we split the treap into
// three pieces  [1, a-1] , [a, b] , [b+1, n] , toggle the reverse flag on the
// middle piece, then merge everything back. Each operation is O(log n).

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;

int lc[MAXN], rc[MAXN], sz[MAXN];
unsigned int pri[MAXN];
char val[MAXN];
bool rev_[MAXN];
int nodeCount = 0;

mt19937 rng(1234567891u);

int newNode(char c) {
    int x = ++nodeCount;
    lc[x] = rc[x] = 0;
    sz[x] = 1;
    pri[x] = rng();
    val[x] = c;
    rev_[x] = false;
    return x;
}

inline void update(int x) {
    if (!x) return;
    sz[x] = 1 + sz[lc[x]] + sz[rc[x]];
}

inline void pushDown(int x) {
    if (x && rev_[x]) {
        swap(lc[x], rc[x]);
        if (lc[x]) rev_[lc[x]] ^= 1;
        if (rc[x]) rev_[rc[x]] ^= 1;
        rev_[x] = false;
    }
}

// Split t so that the first k nodes (in-order) go to l, the rest to r.
void split(int t, int k, int &l, int &r) {
    if (!t) { l = r = 0; return; }
    pushDown(t);
    if (sz[lc[t]] < k) {
        int rr;
        split(rc[t], k - sz[lc[t]] - 1, rc[t], rr);
        l = t;
        r = rr;
    } else {
        int ll;
        split(lc[t], k, ll, lc[t]);
        l = ll;
        r = t;
    }
    update(t);
}

int merge(int l, int r) {
    if (!l || !r) return l | r;
    if (pri[l] > pri[r]) {
        pushDown(l);
        rc[l] = merge(rc[l], r);
        update(l);
        return l;
    } else {
        pushDown(r);
        lc[r] = merge(l, lc[r]);
        update(r);
        return r;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    string s;
    cin >> s;

    int root = 0;
    for (int i = 0; i < n; ++i)
        root = merge(root, newNode(s[i]));

    for (int q = 0; q < m; ++q) {
        int a, b;
        cin >> a >> b;
        int A, B, C, tmp;
        // A = [1, a-1], tmp = [a, n]
        split(root, a - 1, A, tmp);
        // B = [a, b], C = [b+1, n]
        split(tmp, b - a + 1, B, C);
        rev_[B] ^= 1;                 // lazily reverse the middle segment
        root = merge(merge(A, B), C);
    }

    // In-order traversal (iterative, pushing lazy flags down) to print result.
    string out;
    out.reserve(n);
    vector<int> stk;
    stk.reserve(64);
    int cur = root;
    while (cur || !stk.empty()) {
        while (cur) {
            pushDown(cur);
            stk.push_back(cur);
            cur = lc[cur];
        }
        cur = stk.back(); stk.pop_back();
        out.push_back(val[cur]);
        cur = rc[cur];
    }

    cout << out << '\n';
    return 0;
}
