#pragma once
#include <bits/stdc++.h>
using namespace std;
// Sprague-Grundy — lý thuyết trò chơi tổ hợp: XOR-sum của Nim và số Grundy (mex) cho trò trừ que.
// Khi dùng: trò chơi công bằng (impartial) tổng của nhiều ván độc lập; xác định thắng/thua bằng XOR các Grundy.
// ĐPT: nimXor O(số đống); grundySubtraction O(n · |moves|); mex O(|s|). Người đi trước THẮNG <=> XOR Grundy != 0.
// Dùng: nimXor(piles); auto g = grundySubtraction(n, moves); // g[i] = Grundy của trạng thái i que
// Bẫy: chỉ đúng cho trò IMPARTIAL; tổng ván = XOR các Grundy; vị trí THUA (P-position) <=> Grundy = 0.
// CSES: 1098 1099 1729 1730 2207 2208
// Ý nghĩa: XOR-sum của các đống (định lý Sprague-Grundy cho Nim).
// Tham số: piles = kích thước các đống.
// Trả về: XOR tất cả; khác 0 nghĩa người đi trước thắng.
long long nimXor(const vector<long long>& piles) {
    long long x = 0;
    for (long long p : piles) x ^= p;
    return x;
}
// Ý nghĩa: mex (minimum excludant) = số tự nhiên nhỏ nhất KHÔNG thuộc tập.
// Tham số: s = tập các số nguyên không âm (có thể lặp).
// Trả về: số tự nhiên nhỏ nhất không xuất hiện trong s.
int mex(const vector<int>& s) {
    vector<char> seen(s.size() + 1, 0);
    for (int v : s) if (v >= 0 && v < (int)seen.size()) seen[v] = 1;
    int m = 0;
    while (m < (int)seen.size() && seen[m]) m++;
    return m;
}
// Ý nghĩa: tính số Grundy cho trò TRỪ QUE: từ i que được lấy một số 'mv' trong moves.
// Tham số: n = số que tối đa; moves = tập nước đi hợp lệ (số que được lấy mỗi lượt).
// Trả về: mảng g với g[i] = số Grundy của trạng thái i que (g[i]==0 là vị trí THUA).
vector<int> grundySubtraction(int n, const vector<int>& moves) {
    vector<int> g(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        vector<int> reach;
        for (int mv : moves) if (mv <= i) reach.push_back(g[i - mv]);
        g[i] = mex(reach);
    }
    return g;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ sprague-grundy.hpp -o demo && ./demo
int main() {
    printf("Nim [3,4,5] người đi trước thắng? %d\n", (int)(nimXor({3, 4, 5}) != 0));   // 3^4^5=2 -> 1
    auto g = grundySubtraction(10, {1, 3, 4});
    printf("Grundy trò trừ {1,3,4}: ");
    for (int i = 0; i <= 10; i++) printf("%d ", g[i]);   // 0 1 0 1 2 3 2 0 1 0 1
    printf("\n");
    return 0;
}
#endif
