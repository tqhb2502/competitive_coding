// Inversion Sorting  -  CSES task 3140
// https://cses.fi/problemset/task/3140
//
// Interactive: co mot hoan vi an a[1..n]. Moi luot ta in "i j" de dao nguoc
// (reverse) doan [i..j]; grader tra ve so nghich the (inversions) sau thao tac.
// Khi so nghich the = 0 (da sap xep) thi thang. Toi da 4n thao tac.
//
// Chien luoc: (1) khoi phuc hoan vi bang cach reverse tien to va do delta so
// nghich the -> suy ra so nghich the trong moi tien to -> Lehmer code -> hoan
// vi. (2) selection sort bang reverse de sap xep. Tong ~3n <= 4n thao tac.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (!(cin >> n)) return 0;

    // In thao tac reverse [i,j] (1-indexed), flush, doc lai so nghich the.
    auto query = [&](int i, int j) -> long long {
        cout << i << ' ' << j << endl;   // endl -> flush bat buoc cho interactive
        cout.flush();
        long long inv = -1;
        cin >> inv;
        return inv;
    };

    if (n == 1) {
        // Hoan vi [1] da sap xep; thuc hien 1 thao tac khong doi de doc so 0.
        query(1, 1);
        return 0;
    }

    // ---- Pha 1: khoi phuc hoan vi ----
    // Khi reverse doan do dai L, delta = L(L-1)/2 - 2*x voi x = so nghich the
    // ben trong doan (cac cap ca hai dau nam trong doan). Cac cap khac khong doi.
    // Voi tien to [1,i]: reverse roi reverse lai (restore) => mang giu nguyen goc,
    // v2 = T (tong nghich the goc), v1 = T + C(i,2) - 2*x[i].
    //   => x[i] = (T + C(i,2) - v1) / 2   (so nghich the trong tien to i cua mang goc)
    // Neu bat ky lan doc nao = 0 -> mang da sap xep -> thang, ket thuc.
    vector<long long> x(n + 1, 0); // x[i] = nghich the trong tien to do dai i
    for (int i = 2; i <= n; i++) {
        long long v1 = query(1, i);
        if (v1 == 0) return 0;                 // da sap xep -> thang
        long long v2 = query(1, i);            // restore, v2 = T
        if (v2 == 0) return 0;                 // mang goc da sap xep -> thang
        long long T = v2;
        long long Ci = (long long)i * (i - 1) / 2;
        x[i] = (T + Ci - v1) / 2;
    }

    // c[i] = x[i]-x[i-1] = so phan tu dung truoc vi tri i lon hon a[i] (0<=c[i]<=i-1).
    // r[i] = i - c[i] = hang (tang dan) cua a[i] trong i phan tu dau.
    // Chen "phan tu i" vao vi tri r[i]-1 cua danh sach da sap theo gia tri.
    vector<int> rankList;
    rankList.reserve(n);
    for (int i = 1; i <= n; i++) {
        long long ci = (i == 1) ? 0 : (x[i] - x[i - 1]);
        int ri = i - (int)ci;                  // 1..i
        rankList.insert(rankList.begin() + (ri - 1), i);
    }
    // rankList[k] la phan tu (chi so vi tri) co gia tri k+1.
    vector<int> a(n + 1, 0);                    // a[pos] = gia tri
    for (int k = 0; k < n; k++) a[rankList[k]] = k + 1;

    // ---- Pha 2: selection sort bang reverse ----
    // Voi pos = 1..n-1: dua gia tri 'pos' ve vi tri pos bang reverse [pos, q],
    // q la vi tri hien tai cua gia tri pos. Tong <= n-1 thao tac.
    for (int pos = 1; pos <= n - 1; pos++) {
        if (a[pos] == pos) continue;
        int q = pos;
        for (int t = pos; t <= n; t++)
            if (a[t] == pos) { q = t; break; }
        long long inv = query(pos, q);
        reverse(a.begin() + pos, a.begin() + q + 1);
        if (inv == 0) return 0;                 // da sap xep -> thang
    }

    // Luoi an toan: neu chua doc duoc 0 (chi xay ra khi mang da la identity),
    // thuc hien 1 thao tac khong doi de doc so 0.
    query(1, 1);
    return 0;
}
