// Eulerian Subgraphs - CSES 2078
// https://cses.fi/problemset/task/2078
//
// Count edge-subsets where every vertex has even degree (the cycle space over
// GF(2)). Such subsets form a vector space of dimension  d = m - n + c , where c
// is the number of connected components. Answer = 2^(m - n + c) mod 1e9+7.

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

int par[100005];
int rnk[100005];

int findp(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

void unite(int a, int b) {
    a = findp(a);
    b = findp(b);
    if (a == b) return;
    if (rnk[a] < rnk[b]) swap(a, b);
    par[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
}

long long power_mod(long long base, long long exp, long long mod) {
    long long result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    for (int i = 1; i <= n; i++) {
        par[i] = i;
        rnk[i] = 0;
    }

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        unite(a, b);
    }

    // Count connected components (isolated vertices included).
    int c = 0;
    for (int i = 1; i <= n; i++) {
        if (findp(i) == i) c++;
    }

    // Cycle rank = m - n + c  (always >= 0).
    long long rank_ = (long long)m - (long long)n + (long long)c;

    cout << power_mod(2, rank_, MOD) << '\n';
    return 0;
}
