#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

// DSU (union-find) để đếm số thành phần liên thông c của đồ thị.
int par[100005];
int rnk[100005];

// Tìm root với path compression.
int findp(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

// Hợp nhất hai tập theo union by rank.
void unite(int a, int b) {
    a = findp(a);
    b = findp(b);
    if (a == b) return;
    if (rnk[a] < rnk[b]) swap(a, b);
    par[b] = a;
    if (rnk[a] == rnk[b]) rnk[a]++;
}

// Lũy thừa nhanh: tính base^exp mod.
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

    // Khởi tạo mỗi đỉnh là một tập riêng.
    for (int i = 1; i <= n; i++) {
        par[i] = i;
        rnk[i] = 0;
    }

    // Hợp nhất hai đầu mút của từng cạnh.
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        unite(a, b);
    }

    // Đếm số thành phần liên thông (kể cả các đỉnh cô lập).
    int c = 0;
    for (int i = 1; i <= n; i++) {
        if (findp(i) == i) c++;
    }

    // Số chiều cycle space = m - n + c (luôn >= 0).
    long long rank_ = (long long)m - (long long)n + (long long)c;

    // Số Eulerian subgraph = 2^(m - n + c) mod 1e9+7.
    cout << power_mod(2, rank_, MOD) << '\n';
    return 0;
}
