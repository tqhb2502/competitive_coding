#pragma once
#include <bits/stdc++.h>
using namespace std;
// Long-Path Decomposition — đếm số đường đi đúng k cạnh trên cây trong O(n) bằng tái sử dụng mảng depth.
// Khi dùng: duy nhất bài đếm pair/path có khoảng cách CỐ ĐỊNH cần nhanh hơn centroid O(n log n).
// ĐPT: O(n) thời gian và O(n) bộ nhớ; DFS lặp, an toàn cây dây n lớn. Đỉnh 0-based, cây không trọng số.
// Dùng: long long pairs = countPathsOfExactLengthLinear(adj, k); // cặp không thứ tự; k=0 trả n
// Bẫy: adj phải là một CÂY vô hướng; đây là tối ưu chuyên biệt cho đúng MỘT độ dài k, không phải HLD.
// CSES: 2080

// Ý nghĩa: DSU-on-long-chain bằng memory pool; mỗi đường đi được đếm đúng tại LCA hai đầu mút.
// Tham số: adj = danh sách kề cây; target = số cạnh chính xác, target>=0.
// Trả về: số cặp đỉnh không thứ tự cách nhau target cạnh (với target=0 tính n path một đỉnh).
inline long long countPathsOfExactLengthLinear(const vector<vector<int>>& adj, int target) {
    int n = (int)adj.size();
    assert(target >= 0);
    if (n == 0 || target >= n) return 0;

    vector<int> parent(n, -2), order;
    order.reserve(n);
    vector<int> stack = {0};
    parent[0] = -1;
    long long degreeSum = 0;
    for (const auto& edges : adj) degreeSum += (long long)edges.size();
    assert(degreeSum == 2LL * (n - 1));
    while (!stack.empty()) {
        int u = stack.back();
        stack.pop_back();
        order.push_back(u);
        for (int v : adj[u]) {
            assert(0 <= v && v < n);
            if (v == parent[u]) continue;
            assert(parent[v] == -2);  // gặp lại một đỉnh => không phải cây
            parent[v] = u;
            stack.push_back(v);
        }
    }
    assert((int)order.size() == n);

    vector<int> height(n, 0), heavy(n, -1);
    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        int u = *it, p = parent[u];
        if (p >= 0 && height[u] + 1 > height[p]) {
            height[p] = height[u] + 1;
            heavy[p] = u;
        }
    }

    vector<int> pointer(n, 0);
    int poolSize = 0;
    for (int u : order) {
        if (parent[u] >= 0 && heavy[parent[u]] == u) continue;
        int current = u, offset = poolSize;
        poolSize += height[u] + 1;
        while (current != -1) {
            pointer[current] = offset++;
            current = heavy[current];
        }
    }
    assert(poolSize == n);
    vector<long long> distanceCount(poolSize, 0);
    long long answer = 0;

    for (auto it = order.rbegin(); it != order.rend(); ++it) {
        int u = *it, base = pointer[u];
        distanceCount[base] = 1;  // chính u; ô base+d (d>=1) đã thừa kế heavy child
        for (int child : adj[u]) {
            if (parent[child] != u || child == heavy[u]) continue;
            int childBase = pointer[child];
            for (int childDepth = 0; childDepth <= height[child]; ++childDepth) {
                int previousDepth = target - childDepth - 1;
                if (previousDepth < 1) break;
                if (previousDepth <= height[u])
                    answer += distanceCount[childBase + childDepth]
                            * distanceCount[base + previousDepth];
            }
            for (int childDepth = 0; childDepth <= height[child]; ++childDepth)
                distanceCount[base + childDepth + 1]
                    += distanceCount[childBase + childDepth];
        }
        if (target <= height[u]) answer += distanceCount[base + target];
    }
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ long-path-decomposition.hpp -o demo && ./demo
int main() {
    vector<vector<int>> tree(5);
    auto edge = [&](int u, int v) { tree[u].push_back(v); tree[v].push_back(u); };
    edge(0, 1); edge(1, 2); edge(2, 3); edge(3, 4);
    printf("distance 2: %lld; distance 0: %lld\n",
           countPathsOfExactLengthLinear(tree, 2),
           countPathsOfExactLengthLinear(tree, 0));  // 3; 5
    return 0;
}
#endif
