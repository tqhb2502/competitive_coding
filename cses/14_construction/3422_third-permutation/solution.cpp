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
    for (int i = 1; i <= n; i++) ainv[a[i]] = i;   // vị trí có a = giá trị

    // n == 2: bậc cho phép = 0 nên không có hoán vị thứ ba hợp lệ.
    if (n == 2) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    // pi[u] = b[ainv[u]] là derangement trên tập giá trị; phân rã thành các chu
    // trình. Chu trình u_j cấm cặp {u_j, u_{j+1}} tại vị trí ainv[u_j].
    struct Cyc { vector<int> vals, poss; };
    vector<Cyc> big;   // các chu trình dài k >= 3
    vector<Cyc> two;   // các chu trình dài k == 2

    vector<char> vis(n + 1, 0);
    for (int s = 1; s <= n; s++) {
        if (vis[s]) continue;
        Cyc cy;
        int u = s;
        while (!vis[u]) {
            vis[u] = 1;
            cy.vals.push_back(u);
            cy.poss.push_back(ainv[u]);   // vị trí có a = u
            u = b[ainv[u]];               // = pi(u), giá trị kế trong chu trình
        }
        if ((int)cy.vals.size() == 2) two.push_back(move(cy));
        else big.push_back(move(cy));
    }

    vector<int> c(n + 1, 0);

    // Chu trình lớn: xoay một bước, c[poss[j]] = vals[(j-1) mod k] (song ánh nội bộ).
    for (auto &cy : big) {
        int k = (int)cy.vals.size();
        for (int j = 0; j < k; j++)
            c[cy.poss[j]] = cy.vals[(j - 1 + k) % k];
    }

    int m = (int)two.size();
    if (m >= 2) {
        // Nối vòng: nhóm t nhận giá trị của nhóm (t+1) mod m.
        for (int t = 0; t < m; t++) {
            int tn = (t + 1) % m;
            c[two[t].poss[0]] = two[tn].vals[0];
            c[two[t].poss[1]] = two[tn].vals[1];
        }
    } else if (m == 1) {
        // Chỉ một chu trình 2: ghép nó vào chu trình lớn big[0] (chắc chắn tồn tại).
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
    // m == 0: không cần làm gì thêm.

    // Xuất hoán vị c.
    string out;
    out.reserve((size_t)n * 7);
    for (int i = 1; i <= n; i++) {
        out += to_string(c[i]);
        out += (i < n ? ' ' : '\n');
    }
    cout << out;
    return 0;
}
