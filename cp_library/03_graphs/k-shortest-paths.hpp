#pragma once
#include <bits/stdc++.h>
using namespace std;
// K Shortest Paths — k độ dài đường đi ngắn nhất (cho phép lặp đỉnh/cạnh) bằng Dijkstra k-lần-pop.
// Khi dùng: cần k route ngắn nhất trên đồ thị có trọng số không âm, k nhỏ; đường đi không bắt buộc đơn.
// ĐPT: O(k*m log(k*m)) thời gian, O(k*n+m) bộ nhớ trong trường hợp xấu.
// Dùng: auto ans=kShortestWalks(adj, source, target, k); // tăng dần, có thể ít hơn k nếu không đủ
// Bẫy: cạnh phải không âm; kết quả là WALK nên chu trình được phép; phép cộng bão hòa ở KSP_INF để tránh tràn.
// CSES: 1196
const long long KSP_INF = (long long)4e18;

// Ý nghĩa: liệt kê k độ dài walk ngắn nhất từ source tới target theo thứ tự không giảm.
// Tham số: adj[u]={(v,w)} có w>=0; source,target = đỉnh 0-based; k = số đáp án cần (>0).
// Trả về: tối đa k khoảng cách; rỗng nếu target không tới được.
vector<long long> kShortestWalks(const vector<vector<pair<int, long long>>>& adj,
                                 int source, int target, int k) {
    int n = (int)adj.size();
    if (source < 0 || source >= n || target < 0 || target >= n || k <= 0)
        throw invalid_argument("kShortestWalks: tham số không hợp lệ");
    vector<int> popped(n, 0);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, source});
    vector<long long> answer;
    while (!pq.empty() && (int)answer.size() < k) {
        auto [distance, u] = pq.top();
        pq.pop();
        if (popped[u] == k) continue;
        ++popped[u];
        if (u == target) answer.push_back(distance);
        for (auto [v, weight] : adj[u]) {
            if (weight < 0) throw invalid_argument("kShortestWalks: cạnh âm");
            if (popped[v] == k) continue;
            long long next = distance > KSP_INF - weight ? KSP_INF : distance + weight;
            pq.push({next, v});
        }
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ k-shortest-paths.hpp -o demo && ./demo
int main() {
    vector<vector<pair<int, long long>>> adj(4);
    adj[0] = {{1, 1}, {2, 2}};
    adj[1] = {{2, 1}, {3, 4}};
    adj[2] = {{3, 1}, {1, 1}};
    auto ans = kShortestWalks(adj, 0, 3, 3);
    printf("3 walk ngắn nhất:");
    for (long long x : ans) printf(" %lld", x);  // 3 3 5
    printf("\n");
    return 0;
}
#endif
