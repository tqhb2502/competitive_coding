// Third Permutation - CSES 3422
// https://cses.fi/problemset/task/3422
//
// Cho hai hoan vi a, b voi a_i != b_i. Dung hoan vi c sao cho c_i != a_i va
// c_i != b_i. In "IMPOSSIBLE" neu vo nghiem (chi khi n == 2).
//
// Y tuong: pi(a_i) = b_i la mot derangement tren tap gia tri; phan ra thanh
// cac chu trinh. Chu trinh dai >= 3 xoay 1 buoc; cac chu trinh dai 2 phai
// muon gia tri cheo (noi vong neu co >= 2, hoac ghep vao mot chu trinh lon
// neu chi co dung mot). Do phuc tap O(n).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<int> a(n + 1), b(n + 1), ainv(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];
    for (int i = 1; i <= n; i++) ainv[a[i]] = i;

    if (n == 2) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    // pi[u] = b[ainv[u]] la derangement tren gia tri.
    struct Cyc { vector<int> vals, poss; };
    vector<Cyc> big;   // chu trinh dai k >= 3
    vector<Cyc> two;   // chu trinh dai k == 2

    vector<char> vis(n + 1, 0);
    for (int s = 1; s <= n; s++) {
        if (vis[s]) continue;
        Cyc cy;
        int u = s;
        while (!vis[u]) {
            vis[u] = 1;
            cy.vals.push_back(u);
            cy.poss.push_back(ainv[u]);   // vi tri co a = u
            u = b[ainv[u]];               // = pi(u)
        }
        if ((int)cy.vals.size() == 2) two.push_back(move(cy));
        else big.push_back(move(cy));
    }

    vector<int> c(n + 1, 0);

    // Chu trinh lon: xoay c[poss[j]] = vals[(j-1) mod k].
    for (auto &cy : big) {
        int k = (int)cy.vals.size();
        for (int j = 0; j < k; j++)
            c[cy.poss[j]] = cy.vals[(j - 1 + k) % k];
    }

    int m = (int)two.size();
    if (m >= 2) {
        // Noi vong: nhom t nhan gia tri cua nhom (t+1) mod m.
        for (int t = 0; t < m; t++) {
            int tn = (t + 1) % m;
            c[two[t].poss[0]] = two[tn].vals[0];
            c[two[t].poss[1]] = two[tn].vals[1];
        }
    } else if (m == 1) {
        // Ghep chu trinh 2 duy nhat vao chu trinh lon big[0] (chac chan ton tai).
        Cyc &B = big[0];
        int k = (int)B.vals.size();       // k >= 3
        int x = two[0].vals[0], y = two[0].vals[1];
        int P = two[0].poss[0], Q = two[0].poss[1];
        c[P] = B.vals[0];
        c[Q] = B.vals[1];
        c[B.poss[0]] = B.vals[k - 1];
        c[B.poss[1]] = x;
        c[B.poss[2]] = y;
        for (int j = 3; j < k; j++) c[B.poss[j]] = B.vals[j - 1];
    }
    // m == 0: khong can lam gi them.

    string out;
    out.reserve((size_t)n * 7);
    for (int i = 1; i <= n; i++) {
        out += to_string(c[i]);
        out += (i < n ? ' ' : '\n');
    }
    cout << out;
    return 0;
}
