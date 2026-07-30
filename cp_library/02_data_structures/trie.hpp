#pragma once
#include <bits/stdc++.h>
using namespace std;
// Trie — cây tiền tố cho tập từ (bảng chữ cái cố định, mặc định 26 chữ thường 'a'..'z').
// Khi dùng: đếm từ có tiền tố cho trước, kiểm tra tồn tại, word-break DP.
// ĐPT: insert/query O(|s|); bộ nhớ O(tổng độ dài · ALPHA).
// Dùng: Trie t; t.insert("abc"); t.countPrefix("ab"); t.countWord("abc");
// Bẫy: ALPHA và gốc 'a' cố định — đổi bảng chữ cái phải sửa; ký tự ngoài ['a','z'] sẽ hỏng.
// CSES: 1731
struct Trie {
    static const int A = 26;
    struct Node {
        array<int, A> nx;
        int cntPrefix = 0, cntWord = 0;
        // Ý nghĩa: khởi tạo nút mới, đặt mọi con nx về -1 (chưa có cạnh).
        Node() { nx.fill(-1); }
    };
    vector<Node> tr;
    // Ý nghĩa: dựng Trie rỗng với đúng một nút gốc (chỉ số 0).
    Trie() : tr(1) {}
    // Ý nghĩa: thêm từ s vào cây, tạo nút nếu thiếu và tăng cntPrefix dọc đường, cntWord ở nút cuối.
    // Tham số: s = từ cần chèn (các ký tự trong ['a','z']).
    void insert(const string& s) {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (tr[u].nx[c] == -1) { tr[u].nx[c] = (int)tr.size(); tr.push_back(Node()); }
            u = tr[u].nx[c];
            tr[u].cntPrefix++;
        }
        tr[u].cntWord++;
    }
    // Ý nghĩa: đi theo các ký tự của s từ gốc, tìm nút ứng với tiền tố s.
    // Tham số: s = chuỗi tiền tố cần dò.
    // Trả về: chỉ số nút cuối nếu đi hết được, hoặc -1 nếu gặp cạnh thiếu.
    int walk(const string& s) const {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (tr[u].nx[c] == -1) return -1;
            u = tr[u].nx[c];
        }
        return u;
    }
    // Ý nghĩa: đếm số từ đã chèn có s làm tiền tố.
    // Tham số: s = tiền tố cần đếm.
    // Trả về: cntPrefix tại nút của s, hoặc 0 nếu s không tồn tại trong cây.
    int countPrefix(const string& s) const { int u = walk(s); return u < 0 ? 0 : tr[u].cntPrefix; }
    // Ý nghĩa: đếm số lần từ s chính xác đã được chèn vào cây.
    // Tham số: s = từ cần đếm.
    // Trả về: cntWord tại nút của s, hoặc 0 nếu s không tồn tại trong cây.
    int countWord(const string& s) const { int u = walk(s); return u < 0 ? 0 : tr[u].cntWord; }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ trie.hpp -o demo && ./demo
int main() {
    Trie t;
    t.insert("abc");
    t.insert("ab");
    t.insert("abcd");
    printf("countPrefix(ab)=%d  countWord(abc)=%d\n", t.countPrefix("ab"), t.countWord("abc"));
    return 0;
}
#endif
