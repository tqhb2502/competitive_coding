#pragma once
#include <bits/stdc++.h>
using namespace std;
// Binary Trie — trie theo bit cho truy vấn XOR (max XOR, đếm/xóa theo phần tử).
// Khi dùng: tìm max (x XOR y) với y trong tập; duy trì tập số nguyên không âm theo bit.
// ĐPT: insert/erase/maxXor O(B); bộ nhớ O(số phần tử · B). B = số bit (mặc định 30, giá trị < 2^30).
// Dùng: BinaryTrie t; t.insert(x); t.erase(x); t.maxXor(x); // max của (x ^ y) trên tập KHÁC rỗng
// Bẫy: chọn B đủ lớn; maxXor yêu cầu tập khác rỗng; erase phải khớp giá trị đã insert.
// CSES: 1655
struct BinaryTrie {
    static const int B = 30;
    struct Node {
        int nx[2] = {-1, -1};
        int cnt = 0;
    };
    vector<Node> tr;
    // Ý nghĩa: khởi tạo trie với 1 nút gốc rỗng (chưa chứa phần tử nào).
    BinaryTrie() : tr(1) {}
    // Ý nghĩa: thêm delta bản của x vào trie theo từng bit từ cao xuống thấp; delta<0 để xóa.
    // Tham số: x = giá trị cần thêm (không âm, < 2^B); delta = số lượng thêm (mặc định 1, âm để bớt).
    void insert(long long x, int delta = 1) {
        int u = 0;
        for (int b = B - 1; b >= 0; b--) {
            int bit = (int)((x >> b) & 1);
            if (tr[u].nx[bit] == -1) { tr[u].nx[bit] = (int)tr.size(); tr.push_back(Node()); }
            u = tr[u].nx[bit];
            tr[u].cnt += delta;
        }
    }
    // Ý nghĩa: xóa một bản của x khỏi trie (giảm cnt trên đường đi tương ứng).
    // Tham số: x = giá trị cần xóa (phải đã được insert trước đó).
    void erase(long long x) { insert(x, -1); }
    // Ý nghĩa: tìm giá trị lớn nhất của (x XOR y) với y thuộc tập, tham lam chọn bit ngược khi có.
    // Tham số: x = giá trị đem XOR với các phần tử trong tập.
    // Trả về: max của (x ^ y) trên tập (giả định tập KHÁC rỗng).
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

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ binary-trie.hpp -o demo && ./demo
int main() {
    BinaryTrie t;
    t.insert(5);
    t.insert(10);
    t.insert(2);
    printf("maxXor(3) = %lld\n", t.maxXor(3));   // max của (3 ^ y)
    return 0;
}
#endif
