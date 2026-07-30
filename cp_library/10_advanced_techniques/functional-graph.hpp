#pragma once
#include <bits/stdc++.h>
using namespace std;
// Functional Graph — mỗi đỉnh có ĐÚNG MỘT cạnh ra: nhảy k bước, độ dài "đi tới khi gặp lại", khoảng cách a -> b.
// Khi dùng: bài "teleporter"/hàm kế tiếp: đi đúng k bước từ x; số bước tới khi gặp đỉnh đã thăm; đường đi ngắn nhất a -> b.
// ĐPT: dựng O(n log K); kth/dist O(log K); stepsUntilRepeat O(1); bộ nhớ O(n log K) với K = maxK (mặc định 1e9).
// Dùng: FunctionalGraph fg(nxt); fg.kth(v, k); fg.stepsUntilRepeat(v); fg.dist(a, b);
// Bẫy: nxt[v] là ĐÚNG MỘT đỉnh (0-based, cho phép nxt[v]==v); kth chỉ đúng khi k <= maxK; dist trả -1 nếu không tới.
// CSES: 1160 1750 1751
struct FunctionalGraph {
    int n, LOG;
    vector<int> nxt, tail, cycLen, cycId, cycPos;
    vector<char> inCyc;
    vector<vector<int>> up;   // up[k][v] = đỉnh sau 2^k bước từ v
    // Ý nghĩa: dựng bảng nhảy nhị phân và thông tin chu trình (tail/cycLen/cycId/cycPos) cho đồ thị hàm.
    // Tham số: f = mảng kế tiếp (f[v] = đỉnh đi tới từ v, 0-based); maxK = số bước lớn nhất sẽ hỏi (mặc định 1e9).
    explicit FunctionalGraph(const vector<int>& f, long long maxK = 1000000000LL)
        : n((int)f.size()), nxt(f), tail(n, 0), cycLen(n, 0), cycId(n, -1), cycPos(n, -1), inCyc(n, 0) {
        LOG = 1;
        while ((1LL << LOG) <= max((long long)n, max(1LL, maxK))) LOG++;
        up.assign(LOG + 1, vector<int>(n, 0));
        for (int v = 0; v < n; v++) up[0][v] = nxt[v];
        for (int k = 1; k <= LOG; k++)
            for (int v = 0; v < n; v++) up[k][v] = up[k - 1][up[k - 1][v]];
        buildCycles();
    }
    // Ý nghĩa: tìm chu trình của mọi thành phần và khoảng cách tail[v] từ v tới chu trình (duyệt LẶP, không đệ quy).
    void buildCycles() {
        vector<int> state(n, 0), pos(n, -1);   // state: 0 = chưa xét, 1 = đang trên đường đi, 2 = đã xong
        int cycCount = 0;
        for (int s = 0; s < n; s++) {
            if (state[s]) continue;
            vector<int> path;
            int v = s;
            while (state[v] == 0) { state[v] = 1; pos[v] = (int)path.size(); path.push_back(v); v = nxt[v]; }
            if (state[v] == 1) {                       // gặp lại đỉnh TRÊN đường đi hiện tại -> tìm được chu trình mới
                int start = pos[v], len = (int)path.size() - start, cid = cycCount++;
                for (int i = start; i < (int)path.size(); i++) {
                    int u = path[i];
                    inCyc[u] = 1; cycLen[u] = len; cycId[u] = cid; cycPos[u] = i - start; tail[u] = 0;
                }
                for (int i = start - 1; i >= 0; i--) {  // phần đuôi dẫn vào chu trình
                    tail[path[i]] = tail[path[i + 1]] + 1; cycLen[path[i]] = len; cycId[path[i]] = cid;
                }
            } else {                                    // nhập vào phần đã xử lý xong trước đó
                int t = tail[v], cl = cycLen[v], cid = cycId[v];
                for (int i = (int)path.size() - 1; i >= 0; i--) {
                    tail[path[i]] = ++t; cycLen[path[i]] = cl; cycId[path[i]] = cid;
                }
            }
            for (int u : path) state[u] = 2;
        }
    }
    // Ý nghĩa: đỉnh đứng sau đúng k bước kể từ v (nhảy nhị phân).
    // Tham số: v = đỉnh xuất phát; k = số bước (0 <= k <= maxK).
    // Trả về: đỉnh sau k bước.
    int kth(int v, long long k) const {
        for (int i = 0; i <= LOG && v >= 0; i++) if ((k >> i) & 1) v = up[i][v];
        return v;
    }
    // Ý nghĩa: số bước đi từ v cho tới khi gặp lại một đỉnh ĐÃ THĂM (= độ dài đuôi + độ dài chu trình).
    // Tham số: v = đỉnh xuất phát.
    // Trả về: tail[v] + cycLen[v].
    long long stepsUntilRepeat(int v) const { return (long long)tail[v] + cycLen[v]; }
    // Ý nghĩa: số bước ÍT NHẤT để đi từ a tới b theo các cạnh (mỗi đỉnh chỉ có một hướng đi).
    // Tham số: a = đỉnh đầu; b = đỉnh đích.
    // Trả về: số bước nhỏ nhất, hoặc -1 nếu không thể tới b từ a.
    long long dist(int a, int b) const {
        if (a == b) return 0;
        if (!inCyc[b]) {                              // b nằm trên đuôi: a phải nằm phía dưới b trên cùng đường
            long long d = (long long)tail[a] - tail[b];
            if (d <= 0) return -1;
            return kth(a, d) == b ? d : -1;
        }
        int e = kth(a, tail[a]);                      // đỉnh mà a bước vào chu trình
        if (cycId[e] != cycId[b]) return -1;
        int len = cycLen[b];
        int step = ((cycPos[b] - cycPos[e]) % len + len) % len;
        return (long long)tail[a] + step;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ functional-graph.hpp -o demo && ./demo
int main() {
    // 0->1->2->0 (chu trình dài 3), 3->2 (đuôi), 4->4 (chu trình dài 1)
    FunctionalGraph fg(vector<int>{1, 2, 0, 2, 4});
    printf("kth(3,5)=%d ; stepsUntilRepeat(3)=%lld ; dist(3,1)=%lld ; dist(4,0)=%lld\n",
           fg.kth(3, 5), fg.stepsUntilRepeat(3), fg.dist(3, 1), fg.dist(4, 0));   // 0 ; 4 ; 3 ; -1
    return 0;
}
#endif
