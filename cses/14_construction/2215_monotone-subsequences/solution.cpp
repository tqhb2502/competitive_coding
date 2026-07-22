// Monotone Subsequences - CSES 2215
// https://cses.fi/problemset/task/2215
//
// Tao hoan vi cua 1..n co day con don dieu dai nhat = dung k.
// Co nghiem <=> n <= k*k (do de bao dam k <= n).
// Dung k block, moi block tang dan ben trong, dai gia tri giam dan giua cac
// block -> LDS = so block = k, LIS = kich thuoc block lon nhat <= k.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        int n, k;
        cin >> n >> k;

        // Feasible iff n <= k*k (k <= n guaranteed by constraints).
        if ((long long)k * k < n) {
            cout << "IMPOSSIBLE\n";
            continue;
        }

        // Chia n phan tu vao dung k block, kich thuoc deu nhat co the.
        int base = n / k;   // >= 1 vi k <= n
        int rem = n % k;    // rem block dau co kich thuoc base+1

        string out;
        int high = n;       // gia tri lon nhat con lai de gan
        for (int i = 0; i < k; i++) {
            int sz = base + (i < rem ? 1 : 0);
            int low = high - sz + 1;
            for (int v = low; v <= high; v++) {   // in tang dan trong block
                out += to_string(v);
                out.push_back(' ');
            }
            high -= sz;
        }
        out.back() = '\n';  // out khong rong vi n >= 1
        cout << out;
    }
    return 0;
}
