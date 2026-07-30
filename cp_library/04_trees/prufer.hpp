#pragma once
#include <bits/stdc++.h>
using namespace std;
// Mã Prüfer — song ánh giữa CÂY CÓ NHÃN n đỉnh (nhãn 1..n) và dãy n-2 số trong [1, n], mã hoá/giải mã O(n).
// Khi dùng: đề cho dãy Prüfer đòi dựng lại cây (hoặc ngược lại); sinh cây có nhãn ngẫu nhiên ĐỀU; đếm cây có nhãn (Cayley n^(n-2)).
// ĐPT: pruferEncode O(n) (kề CSR + BFS lấy cha + con trỏ lá, KHÔNG sort/heap), pruferDecode O(n), pruferTreeCount O(log n); bộ nhớ O(n).
// Dùng: auto code = pruferEncode(edges, n); // edges = n-1 cạnh 1-based -> dãy dài n-2
//       auto es = pruferDecode(code); // n = code.size()+2, trả n-1 cạnh;  long long t = pruferTreeCount(n, MOD); // n^(n-2) % MOD
// Bẫy: nhãn 1-BASED như CSES và mọi phần tử code phải thuộc [1, n]; n <= 2 -> code rỗng, pruferDecode(rỗng) hiểu là n = 2 nên roundtrip n = 1 mất; pruferEncode giả định đầu vào ĐÚNG là cây (không tự kiểm tra).
// CSES: 1134

// Ý nghĩa: mã hoá cây có nhãn thành dãy Prüfer (lặp n-2 lần: xoá lá có nhãn NHỎ NHẤT, ghi lại đỉnh kề nó).
// Tham số: edges = đúng n-1 cạnh vô hướng của cây, hai đầu là nhãn 1-based trong [1, n] (thứ tự cạnh tuỳ ý);
//          n = số đỉnh.
// Trả về: dãy Prüfer độ dài n-2 (rỗng nếu n <= 2), mỗi phần tử là một nhãn trong [1, n].
vector<int> pruferEncode(const vector<pair<int, int>>& edges, int n) {
    if (n <= 2) return {};
    // Dựng danh sách kề dạng CSR: adjList[adjStart[v] .. adjStart[v+1]) là các đỉnh kề v.
    vector<int> deg(n + 1, 0), adjStart(n + 2, 0), adjList(2 * (size_t)(n - 1));
    for (const auto& e : edges) { deg[e.first]++; deg[e.second]++; }
    for (int v = 1; v <= n; v++) adjStart[v + 1] = adjStart[v] + deg[v];
    {
        vector<int> pos(adjStart.begin(), adjStart.end());
        for (const auto& e : edges) { adjList[pos[e.first]++] = e.second; adjList[pos[e.second]++] = e.first; }
    }
    // BFS lấy cha, gốc là đỉnh n (đỉnh nhãn lớn nhất không bao giờ bị xoá trong n-2 bước).
    vector<int> par(n + 1, 0), order;
    order.reserve(n);
    par[n] = n;                      // par[v] != 0 <=> v đã thăm; gốc trỏ vào chính nó
    order.push_back(n);
    for (size_t i = 0; i < order.size(); i++) {
        int u = order[i];
        for (int j = adjStart[u]; j < adjStart[u + 1]; j++) {
            int v = adjList[j];
            if (par[v] == 0) { par[v] = u; order.push_back(v); }
        }
    }
    // Con trỏ lá: ptr chỉ tăng, quét lá "mới sinh" theo nhãn tăng dần; lá sinh ra dưới ptr được dùng ngay.
    vector<int> code(n - 2), d = deg;
    int ptr = 1;
    while (d[ptr] != 1) ptr++;
    int leaf = ptr;
    for (int i = 0; i < n - 2; i++) {
        int p = par[leaf];
        code[i] = p;
        if (--d[p] == 1 && p < ptr) leaf = p;
        else { ptr++; while (d[ptr] != 1) ptr++; leaf = ptr; }
    }
    return code;
}

// Ý nghĩa: giải mã dãy Prüfer thành các cạnh của cây có nhãn (nghịch đảo của pruferEncode).
// Tham số: code = dãy Prüfer, mỗi phần tử thuộc [1, n] với n = code.size() + 2 (code rỗng -> n = 2).
// Trả về: đúng n-1 cạnh (lá bị xoá, đỉnh kề) nhãn 1-based, theo thứ tự xoá lá; cạnh cuối luôn kết thúc ở đỉnh n.
vector<pair<int, int>> pruferDecode(const vector<int>& code) {
    int n = (int)code.size() + 2;
    vector<int> d(n + 2, 1);         // d[v] = bậc còn lại; v xuất hiện k lần trong code -> bậc k+1 (ô n+1 làm chốt)
    for (int x : code) d[x]++;
    int ptr = 1;
    while (d[ptr] != 1) ptr++;
    int leaf = ptr;
    vector<pair<int, int>> edges;
    edges.reserve(n - 1);
    for (int x : code) {
        edges.emplace_back(leaf, x);
        if (--d[x] == 1 && x < ptr) leaf = x;
        else { ptr++; while (d[ptr] != 1) ptr++; leaf = ptr; }
    }
    edges.emplace_back(leaf, n);     // hai đỉnh còn lại: lá hiện tại và đỉnh n
    return edges;
}

// Ý nghĩa: đếm số CÂY CÓ NHÃN trên n đỉnh — công thức Cayley n^(n-2), hệ quả trực tiếp của song ánh Prüfer.
// Tham số: n = số đỉnh (n >= 1); mod = modulo > 0 (tới ~1e18 vẫn an toàn vì phép nhân ép __int128).
// Trả về: n^(n-2) mod mod; với n <= 2 trả 1 % mod (đúng một cây).
long long pruferTreeCount(long long n, long long mod) {
    long long r = 1 % mod, b = n % mod;
    for (long long e = n - 2; e > 0; e >>= 1) {
        if (e & 1) r = (long long)((__int128)r * b % mod);
        b = (long long)((__int128)b * b % mod);
    }
    return r;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ prufer.hpp -o demo && ./demo
int main() {
    // Cây nhãn 1-based, n = 5: ngôi sao tâm 4 (nối 1, 2, 3) và thêm cạnh 4-5.
    int n = 5;
    vector<pair<int, int>> edges = {{4, 2}, {1, 4}, {5, 4}, {4, 3}};   // thứ tự cạnh/đầu mút tuỳ ý
    vector<int> code = pruferEncode(edges, n);
    printf("code =");
    for (int x : code) printf(" %d", x);
    printf("\n");                                                     // code = 4 4 4
    printf("cạnh khôi phục:");
    for (const auto& e : pruferDecode(code)) printf(" (%d,%d)", e.first, e.second);
    printf("\n");                                                     // (1,4) (2,4) (3,4) (4,5)
    printf("đường đi 1-2-3-4: ");
    for (int x : pruferEncode({{1, 2}, {2, 3}, {3, 4}}, 4)) printf("%d ", x);
    printf("\n");                                                     // 2 3
    printf("số cây có nhãn: n=5 -> %lld, n=10 -> %lld\n",
           pruferTreeCount(5, 1000000007), pruferTreeCount(10, 1000000007));   // 125 (5^3), 100000000 (10^8)
    return 0;
}
#endif
