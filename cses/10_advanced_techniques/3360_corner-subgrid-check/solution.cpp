// Corner Subgrid Check — CSES 3360
// https://cses.fi/problemset/task/3360
//
// Cho luoi n x n voi cac chu cai thuoc k chu cai dau. Voi moi chu cai c ta hoi:
// co ton tai hai hang r1<r2 va hai cot c1<c2 sao cho ca 4 goc
// (r1,c1),(r1,c2),(r2,c1),(r2,c2) deu bang c hay khong (subgrid cao va rong >=2)?
//
// Y tuong: voi moi chu cai, moi khi hai o cung hang cung mang chu cai c ta co mot
// "cap cot" (c1,c2). Neu mot cap cot xuat hien o HAI hang khac nhau -> tim duoc
// hinh chu nhat 4 goc bang c. Ta danh dau cap cot bang mot mang mark[c1*n+c2]
// luu ID cua chu cai da danh dau gan nhat. Khi xu ly chu c, neu gap mot cap cot
// da mang gia tri c -> da co hang truoc danh dau -> YES.
//
// Chan do phuc tap bang bound Zarankiewicz (Kovari–Sos–Turan): mot do thi
// hai phia (hang x cot) khong chua K_{2,2} co toi da ~ n^{3/2} canh, va so cap cot
// PHAN BIET duoc danh dau khong the vuot qua C(n,2). Do do voi moi chu cai, so cap
// duoc xu ly truoc khi buoc collision (hoac ket thuc = NO) <= C(n,2)+O(n). Tong
// cong <= k*C(n,2) ~ 1.2e8 phep, thoa man gioi han thoi gian.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<string> g(n);
    for (int i = 0; i < n; i++) cin >> g[i];

    const long long total = (long long)n * n;

    // CSR: voi moi (hang r, chu cai c) ta luu danh sach cot tang dan.
    // seg = r*k + c ; off[seg]..off[seg+1] la doan cot cua (r,c) trong colBuf.
    vector<int> off((long long)n * k + 1, 0);
    for (int r = 0; r < n; r++) {
        const string &row = g[r];
        int base = r * k;
        for (int col = 0; col < n; col++) {
            int c = row[col] - 'A';
            off[base + c + 1]++;         // dem so luong (dich phai 1 de tien prefix)
        }
    }
    for (int i = 1; i <= n * k; i++) off[i] += off[i - 1]; // prefix -> off[i]=start

    vector<uint16_t> colBuf(total);      // col < n <= 3000 nen vua uint16_t
    {
        vector<int> pos(off.begin(), off.end()); // con tro ghi
        for (int r = 0; r < n; r++) {
            const string &row = g[r];
            int base = r * k;
            for (int col = 0; col < n; col++) {
                int c = row[col] - 'A';
                colBuf[pos[base + c]++] = (uint16_t)col; // cot tang dan trong doan
            }
        }
    }

    // mark[c1*n + c2] = ID chu cai danh dau gan nhat cap cot (c1<c2). Khong reset:
    // moi chu cai co ID rieng nen so sanh == c chi dung khi chinh c da danh dau.
    vector<signed char> mark(total, -1);

    string out;
    out.reserve(k * 4);

    for (int c = 0; c < k; c++) {
        bool found = false;
        for (int r = 0; r < n && !found; r++) {
            int seg = r * k + c;
            int s = off[seg], e = off[seg + 1];
            for (int i = s; i < e && !found; i++) {
                int c1 = colBuf[i];
                long long b = (long long)c1 * n;
                for (int j = i + 1; j < e; j++) {
                    long long p = b + colBuf[j];
                    if (mark[p] == (signed char)c) { found = true; break; }
                    mark[p] = (signed char)c;
                }
            }
        }
        out += (found ? "YES\n" : "NO\n");
    }

    cout << out;
    return 0;
}
