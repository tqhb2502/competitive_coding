#pragma once
#include <bits/stdc++.h>
using namespace std;
// Josephus Problem — n người 1..n xếp vòng tròn, ĐẾM TỪ NGƯỜI 1, cứ người thứ k bị loại, lặp tới hết.
// Khi dùng: đề "vòng tròn, đếm k rồi loại": cần thứ tự bị loại, ai sống sót, hoặc người bị loại thứ q (k = 2).
// ĐPT: josephusOrder O(n log n) (Fenwick + tìm phần tử còn sống thứ i);
//      josephusSurvivor O(k log n) khi k nhỏ, tệ nhất O(n) (mỗi vòng lặp m tăng >= 1); josephusPosition O(log n);
//      bộ nhớ: josephusOrder O(n), hai hàm còn lại O(1).
// Dùng: auto ord = josephusOrder(7, 2);        // {2,4,6,1,5,3,7} — nhãn 1-based, ord.back() = người sống sót
//       josephusSurvivor(1e18, 3);             // ai sống sót, không cần dựng mảng
//       josephusPosition(7, 4);                // = 1: người bị loại thứ 4 khi k = 2
// Bẫy: MỌI nhãn/chỉ số ở đây là 1-based (người 1..n, q = 1..n); luật đếm là "loại người thứ k" nên người bị loại
//      đầu tiên là ((k-1) mod n)+1, sau mỗi lần loại việc đếm tiếp tục từ NGƯỜI NGAY SAU người vừa bị loại.
//      Đề dạng "BỎ QUA k người rồi loại 1 người" (CSES 2163) tương ứng gọi với k+1, KHÔNG phải k.
//      k >= 1 (k = 1 nghĩa là loại lần lượt 1,2,...,n → sống sót là n); n = 0 trả về rỗng / 0.
//      josephusPosition CHỈ đúng cho k = 2 (bù lại chạy O(log n) nên n tới 1e18 vẫn được).
// CSES: 2162 2163 2164
// Ý nghĩa: trả về thứ tự bị loại của toàn bộ n người (mô phỏng nhanh bằng Fenwick trên mảng người còn sống).
// Tham số: n = số người (nhãn 1..n); k = cứ người thứ k bị loại (k >= 1, đếm bắt đầu từ người 1).
// Trả về: mảng n nhãn 1-based theo thứ tự bị loại; phần tử cuối là người sống sót lâu nhất.
vector<int> josephusOrder(int n, int k) {
    if (n <= 0 || k <= 0) return {};
    // bit[i] = số người còn sống trong đoạn Fenwick kết thúc tại i; khởi tạo "tất cả đều sống" trong O(n).
    vector<int> bit(n + 1);
    for (int i = 1; i <= n; i++) bit[i] = i & (-i);
    int pwTop = 1;
    while (pwTop * 2 <= n) pwTop *= 2;
    // Đánh dấu người i đã bị loại (giảm 1 trên mọi đoạn Fenwick chứa i).
    auto kill = [&](int i) { for (; i <= n; i += i & (-i)) bit[i]--; };
    // Tìm người còn sống thứ rank (1-based) bằng nhảy nhị phân trên Fenwick.
    auto aliveAt = [&](long long rank) {
        int pos = 0;
        for (int pw = pwTop; pw > 0; pw >>= 1)
            if (pos + pw <= n && bit[pos + pw] < rank) { pos += pw; rank -= bit[pos]; }
        return pos + 1;
    };
    vector<int> order;
    order.reserve(n);
    long long cur = 1;                                        // hạng (trong số người còn sống) của người ĐẾM ĐẦU TIÊN
    for (int m = n; m >= 1; m--) {
        long long idx = (cur - 1 + (long long)k - 1) % m + 1; // hạng của người bị loại lượt này
        int who = aliveAt(idx);
        order.push_back(who);
        kill(who);
        cur = (idx <= m - 1) ? idx : 1;                       // sau khi xóa, người kế tiếp mang đúng hạng idx (hoặc quay về 1)
    }
    return order;
}
// Ý nghĩa: vị trí người SỐNG SÓT cuối cùng, dùng truy hồi r(m) = (r(m-1) + k) mod m có tăng tốc nhảy nhiều bước.
// Tham số: n = số người (1..n); k = cứ người thứ k bị loại (k >= 1).
// Trả về: nhãn 1-based của người sống sót (0 nếu n <= 0).
long long josephusSurvivor(long long n, long long k) {
    if (n <= 0) return 0;
    if (k <= 1) return n;                     // k = 1: loại 1,2,...,n nên người cuối cùng còn lại là n
    long long r = 0, m = 1;                   // r = đáp án 0-based cho m người; r(1) = 0
    while (m < n) {
        // Trong khi r + (k-1)*j < m thì phép mod chưa xảy ra → nhảy thẳng t bước một lúc.
        long long t = (m - r - 1) / (k - 1);
        if (t > n - m) t = n - m;
        if (t > 0) { r += k * t; m += t; }     // k*t <= 2*m nên không tràn
        else { m++; r = (r + k) % m; }
    }
    return r + 1;
}
// Ý nghĩa: với k = 2, tìm người bị loại thứ q mà không mô phỏng — mỗi bước loại hết người chẵn rồi thu nhỏ bài toán.
// Tham số: n = số người (1..n, tới ~4e18); q = thứ tự lần loại cần biết (1 <= q <= n).
// Trả về: nhãn 1-based của người bị loại thứ q (0 nếu n <= 0 hoặc q ngoài [1, n]); q = n cho người sống sót.
long long josephusPosition(long long n, long long q) {
    if (n <= 0 || q < 1 || q > n) return 0;
    long long mul = 1, add = 0;               // đáp án thật = mul * (đáp án bài con) + add
    while (true) {
        long long half = n / 2;               // half lần loại đầu tiên là các số chẵn 2, 4, ..., 2*half
        if (q <= half) return mul * (2 * q) + add;
        if (n % 2 == 0) {
            // Còn lại các số lẻ 1,3,...,n-1 (= 2i-1) và lượt đếm mới bắt đầu đúng từ số 1.
            add -= mul; mul *= 2;
            n = half; q -= half;
        } else {
            // n lẻ: sau các số chẵn, người bị loại tiếp theo là số 1, rồi còn 3,5,...,n (= 2i+1) đếm từ số 3.
            if (q == half + 1) return mul + add;
            add += mul; mul *= 2;
            n = half; q -= half + 1;
        }
    }
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ josephus.hpp -o demo && ./demo
int main() {
    for (int x : josephusOrder(7, 2)) printf("%d ", x);          // 2 4 6 1 5 3 7
    printf("| survivor(7,2)=%lld\n", josephusSurvivor(7, 2));    // 7
    for (int x : josephusOrder(7, 3)) printf("%d ", x);          // 3 6 2 7 5 1 4  (CSES 2163: bỏ qua 2 → gọi k=3)
    printf("| survivor(7,3)=%lld\n", josephusSurvivor(7, 3));    // 4
    printf("survivor(1e18,3)=%lld ; pos(7,4)=%lld ; pos(1e9,1e9)=%lld\n",
           josephusSurvivor(1000000000000000000LL, 3),           // 774011304974515843
           josephusPosition(7, 4),                               // 1
           josephusPosition(1000000000LL, 1000000000LL));         // 926258177
    return 0;
}
#endif
