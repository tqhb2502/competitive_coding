#pragma once
#include <bits/stdc++.h>
using namespace std;
// Manacher — mọi palindrome con trong O(n): d1 (lẻ) và d2 (chẵn); cho palindrome dài nhất và ĐẾM mọi palindrome con.
// Khi dùng: tìm xâu con đối xứng dài nhất; đếm số xâu con là palindrome; tiền xử lý cho truy vấn palindrome.
// ĐPT: O(n) dựng; longest/count O(n); bộ nhớ O(n). d1[i] = bán kính palindrome LẺ tâm i; d2[i] = palindrome CHẴN tâm (i-1,i).
// Dùng: Manacher M(s); int L = M.longest(); long long c = M.count();
// Bẫy: độ dài palindrome lẻ tâm i = 2*d1[i]-1, chẵn = 2*d2[i]; count đếm cả các palindrome độ dài 1; xâu rỗng -> 0.
// CSES: 1111 3138
struct Manacher {
    vector<int> d1, d2;
    // Ý nghĩa: dựng hai mảng bán kính palindrome (lẻ d1, chẵn d2) cho xâu s bằng thuật toán Manacher.
    // Tham số: s = xâu đầu vào.
    explicit Manacher(const string& s) {
        int n = (int)s.size();
        d1.assign(n, 0);
        for (int i = 0, l = 0, r = -1; i < n; i++) {
            int k = (i > r) ? 1 : min(d1[l + r - i], r - i + 1);
            while (i - k >= 0 && i + k < n && s[i - k] == s[i + k]) k++;
            d1[i] = k;
            if (i + k - 1 > r) { l = i - k + 1; r = i + k - 1; }
        }
        d2.assign(n, 0);
        for (int i = 0, l = 0, r = -1; i < n; i++) {
            int k = (i > r) ? 0 : min(d2[l + r - i + 1], r - i + 1);
            while (i - k - 1 >= 0 && i + k < n && s[i - k - 1] == s[i + k]) k++;
            d2[i] = k;
            if (i + k - 1 > r) { l = i - k; r = i + k - 1; }
        }
    }
    // Ý nghĩa: độ dài xâu con palindrome dài nhất.
    // Trả về: độ dài palindrome dài nhất (0 nếu xâu rỗng).
    int longest() const {
        int best = 0;
        for (int x : d1) best = max(best, 2 * x - 1);
        for (int x : d2) best = max(best, 2 * x);
        return best;
    }
    // Ý nghĩa: tổng số xâu con là palindrome (đếm cả palindrome độ dài 1).
    // Trả về: số palindrome con.
    long long count() const {
        long long c = 0;
        for (int x : d1) c += x;
        for (int x : d2) c += x;
        return c;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ manacher.hpp -o demo && ./demo
int main() {
    Manacher M("aabaa");
    printf("palindrome dài nhất = %d ; số palindrome con = %lld\n", M.longest(), M.count());   // 5 ; 9
    return 0;
}
#endif
