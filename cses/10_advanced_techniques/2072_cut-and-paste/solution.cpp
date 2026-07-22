// Cut and Paste - CSES 2072
// https://cses.fi/problemset/task/2072
//
// Implicit treap (rope): moi thao tac (a,b) cat doan [a,b] va dan vao cuoi xau.
// Dung split/merge theo vi tri de di chuyen ca subtree trong O(log n).

#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;

int lc[MAXN], rc[MAXN], sz[MAXN];
unsigned long long pri[MAXN];
char val[MAXN];
int cnt = 0;

mt19937_64 rng(
    (unsigned long long)chrono::steady_clock::now().time_since_epoch().count());

inline int newNode(char c) {
    int id = ++cnt;
    lc[id] = rc[id] = 0;
    sz[id] = 1;
    pri[id] = rng();
    val[id] = c;
    return id;
}

inline void update(int t) {
    if (t) sz[t] = sz[lc[t]] + sz[rc[t]] + 1;
}

// Tach t thanh (a, b): a giu k phan tu dau (in-order), b giu phan con lai.
void split(int t, int k, int &a, int &b) {
    if (!t) { a = b = 0; return; }
    if (sz[lc[t]] < k) {
        a = t;
        split(rc[t], k - sz[lc[t]] - 1, rc[t], b);
        update(t);
    } else {
        b = t;
        split(lc[t], k, a, lc[t]);
        update(t);
    }
}

// Noi a truoc b, giu max-heap theo priority.
int merge(int a, int b) {
    if (!a) return b;
    if (!b) return a;
    if (pri[a] > pri[b]) {
        rc[a] = merge(rc[a], b);
        update(a);
        return a;
    } else {
        lc[b] = merge(a, lc[b]);
        update(b);
        return b;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    string s;
    cin >> s;

    int root = 0;
    for (int i = 0; i < n; ++i)
        root = merge(root, newNode(s[i]));

    while (m--) {
        int a, b;
        cin >> a >> b;
        int T1, R, A, M;
        split(root, b, T1, R);      // T1 = [1..b], R = [b+1..n]
        split(T1, a - 1, A, M);     // A = [1..a-1], M = [a..b]
        root = merge(merge(A, R), M);
    }

    // Duyet in-order iterative de lay xau ket qua.
    string res;
    res.reserve(cnt);
    static int st[MAXN];
    int top = 0, cur = root;
    while (cur || top) {
        while (cur) { st[top++] = cur; cur = lc[cur]; }
        cur = st[--top];
        res.push_back(val[cur]);
        cur = rc[cur];
    }

    cout << res << '\n';
    return 0;
}
