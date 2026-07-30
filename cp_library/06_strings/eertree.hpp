#pragma once
#include <bits/stdc++.h>
using namespace std;
// Eertree (palindromic tree) — cây palindrome: mỗi node là MỘT palindrome con phân biệt, dựng tăng dần O(n * A).
// Khi dùng: đếm palindrome con PHÂN BIỆT, palindrome dài nhất kết thúc tại mỗi vị trí, tổng số palindrome con.
// ĐPT: O(n * A) (A = 26); bộ nhớ O(n * A). Node 0 = gốc ảo (len -1), node 1 = gốc rỗng (len 0).
// Dùng: Eertree T; for (char c : s) T.addChar(c); T.distinctPalindromes(); T.countAllPalindromes();
// Bẫy: addChar trả độ dài palindrome DÀI NHẤT kết thúc tại ký tự vừa thêm; countAllPalindromes chỉ gọi MỘT lần (làm hỏng cnt).
// CSES: 3138
struct Eertree {
    struct Node { int len, link, cnt; array<int, 26> nxt; };
    vector<Node> t;
    string s;
    int last;
    // Ý nghĩa: khởi tạo eertree với hai gốc: node 0 (len -1) và node 1 (len 0).
    Eertree() {
        t.push_back({-1, 0, 0, {}}); t[0].nxt.fill(-1);
        t.push_back({0, 0, 0, {}});  t[1].nxt.fill(-1);
        last = 1;
    }
    // Ý nghĩa: từ node v, đi theo suffix link tới node mà ký tự trước palindrome khớp ký tự vừa thêm.
    // Tham số: v = node xuất phát.
    // Trả về: node phù hợp để nối palindrome mới.
    int getLink(int v) {
        while (true) {
            int i = (int)s.size() - 2 - t[v].len;
            if (i >= 0 && s[i] == s.back()) return v;
            v = t[v].link;
        }
    }
    // Ý nghĩa: thêm một ký tự vào cuối xâu, cập nhật cây palindrome.
    // Tham số: ch = ký tự (chữ thường 'a'..'z').
    // Trả về: độ dài palindrome dài nhất kết thúc tại ký tự vừa thêm.
    int addChar(char ch) {
        s.push_back(ch);
        int c = ch - 'a';
        int cur = getLink(last);
        if (t[cur].nxt[c] == -1) {
            Node nd; nd.len = t[cur].len + 2; nd.cnt = 0; nd.nxt.fill(-1);
            nd.link = (nd.len == 1) ? 1 : t[getLink(t[cur].link)].nxt[c];
            int now = (int)t.size();
            t.push_back(nd);
            t[cur].nxt[c] = now;
        }
        last = t[cur].nxt[c];
        t[last].cnt++;
        return t[last].len;
    }
    // Ý nghĩa: số palindrome con PHÂN BIỆT của xâu đã thêm.
    // Trả về: số node trừ hai gốc = số palindrome phân biệt.
    long long distinctPalindromes() const { return (long long)t.size() - 2; }
    // Ý nghĩa: tổng số xâu con là palindrome (kể cả trùng lặp) bằng cách lan cnt theo suffix link.
    // Trả về: tổng số palindrome con (chỉ gọi MỘT lần vì làm thay đổi cnt).
    long long countAllPalindromes() {
        for (int v = (int)t.size() - 1; v > 1; v--) t[t[v].link].cnt += t[v].cnt;
        long long total = 0;
        for (int v = 2; v < (int)t.size(); v++) total += t[v].cnt;
        return total;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ eertree.hpp -o demo && ./demo
int main() {
    Eertree T;
    for (char c : string("aabaa")) T.addChar(c);
    // palindrome phân biệt: a aa b aba aabaa = 5
    printf("palindrome phân biệt 'aabaa' = %lld ; tổng palindrome con = %lld\n",
           T.distinctPalindromes(), T.countAllPalindromes());   // 5 ; 9
    return 0;
}
#endif
