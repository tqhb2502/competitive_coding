#pragma once
#include <bits/stdc++.h>
using namespace std;
// Trie — cây tiền tố cho tập từ (bảng chữ cái cố định, mặc định 26 chữ thường 'a'..'z').
// Khi dùng: đếm từ có tiền tố cho trước, kiểm tra tồn tại, word-break DP.
// ĐPT: insert/query O(|s|); bộ nhớ O(tổng độ dài · ALPHA).
// Dùng: Trie t; t.insert("abc"); t.countPrefix("ab"); t.countWord("abc");
// Bẫy: ALPHA và gốc 'a' cố định — đổi bảng chữ cái phải sửa; ký tự ngoài ['a','z'] sẽ hỏng.
struct Trie {
    static const int A = 26;
    struct Node {
        array<int, A> nx;
        int cntPrefix = 0, cntWord = 0;
        Node() { nx.fill(-1); }
    };
    vector<Node> tr;
    Trie() : tr(1) {}
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
    int walk(const string& s) const {
        int u = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (tr[u].nx[c] == -1) return -1;
            u = tr[u].nx[c];
        }
        return u;
    }
    int countPrefix(const string& s) const { int u = walk(s); return u < 0 ? 0 : tr[u].cntPrefix; }
    int countWord(const string& s) const { int u = walk(s); return u < 0 ? 0 : tr[u].cntWord; }
};
