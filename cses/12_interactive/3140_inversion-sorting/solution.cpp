#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (!(cin >> n)) return 0;

    // In thao tác reverse [i,j] (1-indexed), flush, rồi đọc lại số nghịch thế.
    auto query = [&](int i, int j) -> long long {
        cout << i << ' ' << j << endl;   // endl -> flush bắt buộc cho interactive
        cout.flush();
        long long inv = -1;
        cin >> inv;
        return inv;
    };

    if (n == 1) {
        // Hoán vị [1] đã sắp xếp; thực hiện 1 thao tác không đổi để đọc số 0.
        query(1, 1);
        return 0;
    }

    // ---- Pha 1: khôi phục hoán vị ----
    // Khi reverse đoạn độ dài L, delta = L(L-1)/2 - 2*x với x = số nghịch thế bên
    // trong đoạn (các cặp cả hai đầu nằm trong đoạn); các cặp khác không đổi.
    // Với tiền tố [1,i]: reverse rồi reverse lại (restore) => mảng giữ nguyên gốc,
    // v2 = T (tổng nghịch thế gốc), v1 = T + C(i,2) - 2*x[i].
    //   => x[i] = (T + C(i,2) - v1) / 2   (số nghịch thế trong tiền tố i của mảng gốc)
    // Nếu bất kỳ lần đọc nào = 0 -> mảng đã sắp xếp -> thắng, kết thúc.
    vector<long long> x(n + 1, 0); // x[i] = nghịch thế trong tiền tố độ dài i
    for (int i = 2; i <= n; i++) {
        long long v1 = query(1, i);
        if (v1 == 0) return 0;                 // đã sắp xếp -> thắng
        long long v2 = query(1, i);            // restore, v2 = T
        if (v2 == 0) return 0;                 // mảng gốc đã sắp xếp -> thắng
        long long T = v2;
        long long Ci = (long long)i * (i - 1) / 2;
        x[i] = (T + Ci - v1) / 2;
    }

    // c[i] = x[i]-x[i-1] = số phần tử đứng trước vị trí i lớn hơn a[i] (0<=c[i]<=i-1).
    // r[i] = i - c[i] = hạng (tăng dần) của a[i] trong i phần tử đầu.
    // Chèn "phần tử i" vào vị trí r[i]-1 của danh sách đã sắp theo giá trị.
    vector<int> rankList;
    rankList.reserve(n);
    for (int i = 1; i <= n; i++) {
        long long ci = (i == 1) ? 0 : (x[i] - x[i - 1]);
        int ri = i - (int)ci;                  // 1..i
        rankList.insert(rankList.begin() + (ri - 1), i);
    }
    // rankList[k] là phần tử (chỉ số vị trí) có giá trị k+1.
    vector<int> a(n + 1, 0);                    // a[pos] = giá trị
    for (int k = 0; k < n; k++) a[rankList[k]] = k + 1;

    // ---- Pha 2: selection sort bằng reverse ----
    // Với pos = 1..n-1: đưa giá trị 'pos' về vị trí pos bằng reverse [pos, q],
    // q là vị trí hiện tại của giá trị pos. Tổng <= n-1 thao tác.
    for (int pos = 1; pos <= n - 1; pos++) {
        if (a[pos] == pos) continue;
        int q = pos;
        for (int t = pos; t <= n; t++)
            if (a[t] == pos) { q = t; break; }
        long long inv = query(pos, q);
        reverse(a.begin() + pos, a.begin() + q + 1);
        if (inv == 0) return 0;                 // đã sắp xếp -> thắng
    }

    // Lưới an toàn: nếu chưa đọc được 0 (chỉ xảy ra khi mảng đã là identity),
    // thực hiện 1 thao tác không đổi để đọc số 0.
    query(1, 1);
    return 0;
}
