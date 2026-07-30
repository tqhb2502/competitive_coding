#pragma once
#include <bits/stdc++.h>
using namespace std;
// Bellman-Ford — đường đi ngắn nhất cho phép cạnh ÂM; phát hiện và dựng chu trình âm, O(n * m).
// Khi dùng: có cạnh trọng số âm; cần biết đỉnh bị chu trình âm chi phối hoặc cần xuất chính chu trình âm.
// ĐPT: O(n * m); bộ nhớ O(n). edges = {(u, v, w)} cạnh CÓ HƯỚNG u->v; BF_INF = không tới.
// Dùng: auto dist = bellmanFord(n, edges, s); // dist[v]==BF_INF không tới; dist[v]==-BF_INF nếu bị chu trình âm
//       auto cycle = findNegativeCycle(n, edges); // cycle.front()==cycle.back(), rỗng nếu không có
// Bẫy: mọi tổng đường đi hữu hạn cần nằm NGHIÊM NGẶT trong (-BF_INF, BF_INF), vì hai biên là sentinel;
//       findNegativeCycle xét TOÀN đồ thị (siêu nguồn); cạnh âm trên đồ thị VÔ HƯỚNG tạo chu trình âm.
// CSES: 1197 1673
const long long BF_INF = 4'000'000'000'000'000'000LL;

// Ý nghĩa: cộng có chặn để phép relax không gây signed overflow.
long long bfClampedAdd(long long a, long long b) {
    if (a == BF_INF || a == -BF_INF) return a;
    __int128 sum = (__int128)a + b;
    if (sum > BF_INF) return BF_INF;
    if (sum < -BF_INF) return -BF_INF;
    return (long long)sum;
}

// Ý nghĩa: tính đường đi ngắn nhất từ s tới mọi đỉnh với cạnh có thể âm; đánh dấu đỉnh bị chu trình âm.
// Tham số: n = số đỉnh (0-based); edges = danh sách cạnh CÓ HƯỚNG (u, v, w) u->v trọng số w; s = đỉnh nguồn.
// Trả về: dist[v] = khoảng cách ngắn nhất; BF_INF nếu không tới được; -BF_INF nếu bị chu trình âm chi phối.
vector<long long> bellmanFord(int n, const vector<tuple<int, int, long long>>& edges, int s) {
    vector<long long> dist(n, BF_INF);
    dist[s] = 0;
    for (int i = 0; i < n - 1; i++) {
        bool changed = false;
        for (auto [u, v, w] : edges) {
            if (dist[u] == BF_INF) continue;
            long long candidate = bfClampedAdd(dist[u], w);
            if (candidate < dist[v]) {
                dist[v] = candidate;
                changed = true;
            }
        }
        if (!changed) break;
    }
    // Chạy thêm n vòng: cạnh còn nới lỏng được -> v bị chu trình âm chi phối; lan -BF_INF.
    for (int i = 0; i < n; i++)
        for (auto [u, v, w] : edges) {
            if (dist[u] == BF_INF) continue;
            if (dist[u] == -BF_INF || bfClampedAdd(dist[u], w) < dist[v])
                dist[v] = -BF_INF;
        }
    return dist;
}

// Ý nghĩa: tìm và dựng MỘT chu trình âm ở bất kỳ thành phần liên thông nào.
// Kỹ thuật: khởi tạo dist=0 cho mọi đỉnh (siêu nguồn), lưu parent của lần relax thứ n,
//           lùi n bước để chắc chắn vào chu trình rồi lần parent cho tới khi quay lại.
// Trả về: dãy đỉnh theo chiều cạnh, đỉnh đầu lặp lại ở cuối; rỗng nếu toàn đồ thị không có chu trình âm.
vector<int> findNegativeCycle(int n, const vector<tuple<int, int, long long>>& edges) {
    vector<__int128> dist(n, 0);
    vector<int> parent(n, -1);
    int last = -1;
    const __int128 NEG_LIMIT = -((__int128)1 << 120);
    for (int i = 0; i < n; i++) {
        last = -1;
        for (auto [u, v, w] : edges) {
            __int128 candidate = max(NEG_LIMIT, dist[u] + (__int128)w);
            if (candidate < dist[v]) {
                dist[v] = candidate;
                parent[v] = u;
                last = v;
            }
        }
    }
    if (last == -1) return {};

    int inside = last;
    for (int i = 0; i < n; i++) {
        if (inside == -1) return {};
        inside = parent[inside];
    }
    if (inside == -1) return {};

    vector<int> cycle;
    int v = inside;
    do {
        cycle.push_back(v);
        v = parent[v];
        if (v == -1 || (int)cycle.size() > n + 1) return {};
    } while (v != inside);
    cycle.push_back(inside);
    reverse(cycle.begin(), cycle.end());
    return cycle;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ bellman-ford.hpp -o demo && ./demo
int main() {
    vector<tuple<int, int, long long>> edges = {{0, 1, 4}, {0, 2, 5}, {1, 2, -3}, {2, 3, 2}};
    auto dist = bellmanFord(4, edges, 0);
    printf("Bellman-Ford: khoảng cách 0 -> 3 = %lld\n", dist[3]);   // 3
    vector<tuple<int, int, long long>> neg = {{0, 1, 1}, {1, 2, -3}, {2, 0, 1}};
    printf("chu trình âm:");
    for (int v : findNegativeCycle(3, neg)) printf(" %d", v);
    printf("\n");
    return 0;
}
#endif
