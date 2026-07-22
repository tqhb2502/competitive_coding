// Creating Offices - CSES 1752
// https://cses.fi/problemset/task/1752
//
// Maximum d-packing trên cây: chọn tập đỉnh lớn nhất sao cho khoảng cách đôi
// một >= d. Greedy theo độ sâu giảm dần + BFS lan có cắt tỉa để duy trì
// khoảng cách tới văn phòng gần nhất. Tất cả đều dùng BFS lặp (iterative).

#include <bits/stdc++.h>
using namespace std;

// Fast reader (buffered, portable; chỉ đọc số nguyên không âm, đủ cho bài này).
static char ibuf[1 << 22];
static int ipos = 0, ilen = 0;
static inline int gc() {
    if (ipos == ilen) { ilen = (int)fread(ibuf, 1, sizeof(ibuf), stdin); ipos = 0; if (ilen == 0) return -1; }
    return ibuf[ipos++];
}
static inline int readInt() {
    int c = gc();
    while (c != -1 && (c < '0' || c > '9')) c = gc();
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return x;
}

int main() {
    int n = readInt();
    int d = readInt();

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a = readInt();
        int b = readInt();
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // BFS từ đỉnh 1 để lấy thứ tự theo độ sâu tăng dần.
    vector<int> order;
    order.reserve(n);
    vector<char> vis(n + 1, 0);
    order.push_back(1);
    vis[1] = 1;
    for (size_t i = 0; i < order.size(); ++i) {
        int u = order[i];
        for (int v : adj[u])
            if (!vis[v]) { vis[v] = 1; order.push_back(v); }
    }

    const int INF = INT_MAX;
    vector<int> nearest(n + 1, INF); // khoảng cách tới văn phòng gần nhất đã đặt

    vector<int> offices;
    vector<int> bfsq;
    bfsq.reserve(n);

    // Duyệt theo độ sâu GIẢM DẦN (đảo ngược thứ tự BFS).
    for (int idx = (int)order.size() - 1; idx >= 0; --idx) {
        int v = order[idx];
        if (nearest[v] >= d) {
            // Đặt văn phòng tại v.
            offices.push_back(v);
            // BFS lan có cắt tỉa cập nhật nearest[] trong bán kính d-1.
            bfsq.clear();
            nearest[v] = 0;
            bfsq.push_back(v);
            for (size_t i = 0; i < bfsq.size(); ++i) {
                int x = bfsq[i];
                int nd = nearest[x] + 1; // khoảng cách từ v tới hàng xóm của x
                if (nd >= d) continue;   // ngoài bán kính cần quan tâm
                for (int y : adj[x]) {
                    if (nd < nearest[y]) {
                        nearest[y] = nd;
                        bfsq.push_back(y);
                    }
                }
            }
        }
    }

    // Xuất kết quả (buffered).
    string out;
    out.reserve(offices.size() * 7 + 16);
    out += to_string((long long)offices.size());
    out += '\n';
    for (size_t i = 0; i < offices.size(); ++i) {
        if (i) out += ' ';
        out += to_string(offices[i]);
    }
    out += '\n';
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
