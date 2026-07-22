// Number of Subset Xors — CSES 3211
// https://cses.fi/problemset/task/3211
//
// Đếm số giá trị XOR phân biệt của mọi tập con của mảng.
// Tập các giá trị này chính là span của các vector trên GF(2),
// có đúng 2^r phần tử với r = rank của XOR linear basis.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    const int B = 30; // x_i <= 1e9 < 2^30 nên 30 bit là đủ
    array<int, B> basis{};
    basis.fill(0);
    int rank = 0;

    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        // Thử chèn x vào linear basis
        for (int b = B - 1; b >= 0; --b) {
            if (!((x >> b) & 1)) continue;      // bit b không set -> bỏ qua
            if (!basis[b]) {                    // vị trí trống -> thêm phần tử basis mới
                basis[b] = x;
                ++rank;
                break;
            }
            x ^= basis[b];                      // khử bit cao nhất, tiếp tục
        }
        // Nếu x bị khử về 0: phụ thuộc tuyến tính, không tăng rank
    }

    // Số giá trị XOR phân biệt = 2^rank (bao gồm cả tập con rỗng cho giá trị 0)
    cout << (1LL << rank) << '\n';
    return 0;
}
