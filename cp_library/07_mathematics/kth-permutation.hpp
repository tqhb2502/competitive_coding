#pragma once
#include <bits/stdc++.h>
using namespace std;
// Kth Permutation / Permutation Rank — hệ cơ số giai thừa: hoán vị thứ k <-> hạng của hoán vị (hai hàm nghịch đảo nhau).
// Khi dùng: đề cho k và hỏi hoán vị thứ k theo thứ tự từ điển, hoặc cho hoán vị và hỏi nó đứng thứ mấy.
// ĐPT: kthPermutation O(n^2) (mỗi bước xóa 1 phần tử khỏi danh sách còn trống); permutationRank O(n^2);
//      bộ nhớ O(n). Với n <= 20 thì n^2 <= 400 nên O(n^2) là quá đủ, không cần BIT.
// Dùng: auto p = kthPermutation(4, 7);        // p = {1, 0, 3, 2} — hoán vị thứ 7 (0-based) của 0..3
//       long long k = permutationRank(p);     // k = 7 — hạng 0-based, đúng nghịch đảo của kthPermutation
// Bẫy: k và hạng đều 0-BASED (hoán vị nhỏ nhất 0,1,...,n-1 có hạng 0); phần tử là 0..n-1 (0-based),
//      đề hỏi 1..n thì cộng 1 vào từng phần tử khi in. Giả định đầu vào: 0 <= n <= 20 và 0 <= k < n!;
//      permutationRank giả định p là hoán vị của 0..n-1 (đủ mặt, không trùng) — hàm KHÔNG tự kiểm tra.
//      TRÀN SỐ: 20! = 2432902008176640000 vừa long long, 21! = 5.1e19 đã tràn — vì vậy CHẶN n <= 20.
//      Muốn n lớn hơn thì đổi long long -> __int128 cho bảng fact và cho k/hạng, khi đó tối đa n <= 33
//      (33! ~ 8.68e36 < 1.70e38, còn 34! ~ 2.95e38 đã tràn __int128).
//      n = 0 hợp lệ: kthPermutation(0, 0) trả về vector rỗng, permutationRank({}) = 0 (vì 0! = 1).
// CSES: 3397
// Ý nghĩa: dựng hoán vị thứ k (0-based) của dãy 0..n-1 theo thứ tự từ điển, bằng cách khai triển k trong hệ cơ số giai thừa.
// Tham số: n = độ dài hoán vị (giả định 0 <= n <= 20 để n! không tràn long long); k = hạng 0-based, giả định 0 <= k < n!.
// Trả về: vector độ dài n là hoán vị của 0..n-1 đứng thứ k theo thứ tự từ điển.
vector<int> kthPermutation(int n, long long k) {
    vector<long long> fact(n + 1, 1);          // fact[i] = i!, an toàn khi n <= 20
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
    vector<int> avail(n);                      // các giá trị chưa dùng, luôn giữ thứ tự TĂNG
    iota(avail.begin(), avail.end(), 0);
    vector<int> res;
    res.reserve(n);
    for (int i = n; i >= 1; i--) {
        // Còn i vị trí trống -> mỗi lựa chọn cho vị trí hiện tại phủ đúng (i-1)! hoán vị liên tiếp.
        long long block = fact[i - 1];
        int idx = (int)(k / block);             // chữ số thứ i trong hệ cơ số giai thừa, 0 <= idx < i
        k %= block;
        res.push_back(avail[idx]);
        avail.erase(avail.begin() + idx);
    }
    return res;
}
// Ý nghĩa: tính hạng (0-based) của một hoán vị theo thứ tự từ điển — nghịch đảo của kthPermutation.
// Tham số: p = hoán vị của 0..n-1 (0-based, không trùng lặp), giả định độ dài n <= 20.
// Trả về: số hoán vị của 0..n-1 nhỏ hơn p theo thứ tự từ điển, tức hạng 0-based của p (nằm trong [0, n!)).
long long permutationRank(const vector<int>& p) {
    int n = (int)p.size();
    vector<long long> fact(n + 1, 1);          // fact[i] = i!, an toàn khi n <= 20
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i;
    vector<char> used(n, 0);
    long long rnk = 0;
    for (int i = 0; i < n; i++) {
        // Đếm giá trị CHƯA dùng và nhỏ hơn p[i]: chọn mỗi giá trị đó cho ra (n-1-i)! hoán vị đứng trước p.
        int smaller = 0;
        for (int v = 0; v < p[i]; v++) if (!used[v]) smaller++;
        rnk += (long long)smaller * fact[n - 1 - i];
        used[p[i]] = 1;
    }
    return rnk;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ kth-permutation.hpp -o demo && ./demo
int main() {
    vector<int> p = kthPermutation(4, 7);
    printf("hoán vị thứ 7 của 0..3 =");
    for (int x : p) printf(" %d", x);                                       // 1 0 3 2
    printf("\nhạng của nó = %lld\n", permutationRank(p));                   // 7
    printf("hoán vị đầu/cuối của 0..2: ");
    for (int x : kthPermutation(3, 0)) printf("%d", x);                     // 012
    printf(" / ");
    for (int x : kthPermutation(3, 5)) printf("%d", x);                     // 210
    vector<int> desc(20);
    for (int i = 0; i < 20; i++) desc[i] = 19 - i;
    printf("\nhạng của hoán vị giảm dần độ dài 20 = %lld\n", permutationRank(desc));  // 2432902008176639999 = 20!-1
    printf("n = 0: size=%d hạng=%lld\n", (int)kthPermutation(0, 0).size(), permutationRank({}));  // 0 0
    return 0;
}
#endif
