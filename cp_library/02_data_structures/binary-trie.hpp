#pragma once
#include <bits/stdc++.h>
using namespace std;
// Binary Trie — trie theo bit cho truy vấn XOR (max XOR, đếm/xóa theo phần tử).
// Khi dùng: tìm max (x XOR y) với y trong tập; duy trì tập số nguyên không âm theo bit.
// ĐPT: insert/erase/maxXor O(B); bộ nhớ O(số phần tử · B). B = số bit (mặc định 30, giá trị < 2^30).
// Dùng: BinaryTrie t; t.insert(x); t.erase(x); t.maxXor(x); // max của (x ^ y) trên tập KHÁC rỗng
// Bẫy: chọn B đủ lớn; maxXor yêu cầu tập khác rỗng; erase phải khớp giá trị đã insert.
struct BinaryTrie {
    static const int B = 30;
    struct Node {
        int nx[2] = {-1, -1};
        int cnt = 0;
    };
    vector<Node> tr;
    BinaryTrie() : tr(1) {}
    void insert(long long x, int delta = 1) {
        int u = 0;
        for (int b = B - 1; b >= 0; b--) {
            int bit = (int)((x >> b) & 1);
            if (tr[u].nx[bit] == -1) { tr[u].nx[bit] = (int)tr.size(); tr.push_back(Node()); }
            u = tr[u].nx[bit];
            tr[u].cnt += delta;
        }
    }
    void erase(long long x) { insert(x, -1); }
    long long maxXor(long long x) const {  // giả định tập KHÁC rỗng
        int u = 0;
        long long res = 0;
        for (int b = B - 1; b >= 0; b--) {
            int bit = (int)((x >> b) & 1), want = bit ^ 1;
            if (tr[u].nx[want] != -1 && tr[tr[u].nx[want]].cnt > 0) {
                res |= (1LL << b);
                u = tr[u].nx[want];
            } else {
                u = tr[u].nx[bit];
            }
        }
        return res;
    }
};
