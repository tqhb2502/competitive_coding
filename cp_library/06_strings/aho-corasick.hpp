#pragma once
#include <bits/stdc++.h>
using namespace std;
// Aho-Corasick — khớp NHIỀU mẫu cùng lúc: trie + liên kết fail (automaton), O(tổng|mẫu| + |văn bản|).
// Khi dùng: đếm/định vị nhiều mẫu trong một văn bản; mẫu nào xuất hiện; đếm số lần mỗi mẫu (kể cả chồng lấn).
// ĐPT: build O(A * số_node) (A = 26), countOccurrences O(|t| + số_node); bộ nhớ O(A * số_node). Bảng chữ 'a'..'z'.
// Dùng: AhoCorasick ac; ac.addPattern(p); ac.build(); auto cnt = ac.countOccurrences(t); // cnt[i] = số lần mẫu i
// Bẫy: gọi addPattern hết rồi mới build (build biến trie thành automaton); mẫu trùng -> đếm riêng từng bản; chữ thường.
// CSES: 2102 2103
struct AhoCorasick {
    vector<array<int, 26>> nxt;   // trước build: cạnh trie; sau build: hàm chuyển automaton
    vector<int> fail, endCnt, termNode;
    // Ý nghĩa: tạo automaton rỗng với node gốc (0).
    AhoCorasick() { newNode(); }
    // Ý nghĩa: cấp phát một node mới (mọi cạnh = -1) và trả về chỉ số của nó.
    // Trả về: chỉ số node vừa tạo.
    int newNode() {
        nxt.push_back({}); nxt.back().fill(-1);
        fail.push_back(0); endCnt.push_back(0);
        return (int)nxt.size() - 1;
    }
    // Ý nghĩa: thêm một mẫu vào trie và ghi node kết thúc của nó.
    // Tham số: p = mẫu (chữ thường 'a'..'z').
    // Trả về: chỉ số node kết thúc mẫu.
    int addPattern(const string& p) {
        int u = 0;
        for (char ch : p) { int c = ch - 'a'; if (nxt[u][c] < 0) nxt[u][c] = newNode(); u = nxt[u][c]; }
        endCnt[u]++; termNode.push_back(u);
        return u;
    }
    // Ý nghĩa: dựng liên kết fail và hoàn thiện hàm chuyển automaton bằng BFS (gọi sau khi thêm hết mẫu).
    void build() {
        queue<int> q;
        for (int c = 0; c < 26; c++) {
            if (nxt[0][c] < 0) nxt[0][c] = 0;
            else { fail[nxt[0][c]] = 0; q.push(nxt[0][c]); }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int c = 0; c < 26; c++) {
                int v = nxt[u][c];
                if (v < 0) nxt[u][c] = nxt[fail[u]][c];
                else { fail[v] = nxt[fail[u]][c]; q.push(v); }
            }
        }
    }
    // Ý nghĩa: đếm số lần xuất hiện (kể cả chồng lấn) của mỗi mẫu trong văn bản t.
    // Tham số: t = văn bản (chữ thường).
    // Trả về: mảng theo THỨ TỰ thêm mẫu, phần tử i = số lần mẫu thứ i xuất hiện trong t.
    vector<long long> countOccurrences(const string& t) {
        int n = (int)nxt.size();
        vector<long long> hit(n, 0);
        int u = 0;
        for (char ch : t) { u = nxt[u][ch - 'a']; hit[u]++; }
        vector<vector<int>> children(n);
        for (int v = 1; v < n; v++) children[fail[v]].push_back(v);
        vector<int> order;
        order.reserve(n);
        queue<int> q; q.push(0);
        while (!q.empty()) { int x = q.front(); q.pop(); order.push_back(x); for (int y : children[x]) q.push(y); }
        for (int i = (int)order.size() - 1; i >= 0; i--) { int v = order[i]; if (v) hit[fail[v]] += hit[v]; }
        vector<long long> res;
        for (int node : termNode) res.push_back(hit[node]);
        return res;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ aho-corasick.hpp -o demo && ./demo
int main() {
    AhoCorasick ac;
    ac.addPattern("he"); ac.addPattern("she"); ac.addPattern("hers"); ac.addPattern("his");
    ac.build();
    auto cnt = ac.countOccurrences("ahishers");   // he, she, hers, his: mỗi mẫu xuất hiện đúng 1 lần
    printf("he=%lld she=%lld hers=%lld his=%lld\n", cnt[0], cnt[1], cnt[2], cnt[3]);   // 1 1 1 1
    return 0;
}
#endif
