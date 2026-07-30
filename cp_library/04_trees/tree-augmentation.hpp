#pragma once
#include <bits/stdc++.h>
using namespace std;
// Tree Augmentation — thêm tối thiểu ceil(leaves/2) cạnh để cây không còn cầu (2-edge-connected).
// Khi dùng: nối cặp lá của cây để sau khi thêm cạnh, bỏ bất kỳ một cạnh cũ vẫn liên thông.
// ĐPT: O(n); bộ nhớ O(n). Đỉnh 0-based.
// Dùng: auto extra = augmentTreeNoBridges(adj); // thêm các cạnh extra vào cây
// Bẫy: adj phải là một cây vô hướng đối xứng; n=2 trả một cạnh song song (đề phải cho phép multiedge).
// CSES: 1704
// Ý nghĩa: ghép các lá theo nửa vòng DFS để mọi cạnh cây nằm trên ít nhất một chu trình.
// Tham số: adj = danh sách kề của cây vô hướng liên thông.
// Trả về: đúng ceil(L/2) cạnh cần thêm; rỗng nếu n<=1.
vector<pair<int, int>> augmentTreeNoBridges(const vector<vector<int>>& adj) {
    int n = (int)adj.size();
    if (n <= 1) return {};
    long long degreeSum = 0;
    for (int u = 0; u < n; ++u) {
        degreeSum += adj[u].size();
        for (int v : adj[u]) if (v < 0 || v >= n)
            throw invalid_argument("augmentTreeNoBridges: đỉnh ngoài miền");
    }
    if (degreeSum != 2LL * (n - 1))
        throw invalid_argument("augmentTreeNoBridges: số cạnh không phải cây");
    if (n == 2) return {{0, 1}};
    int root = 0;
    while ((int)adj[root].size() == 1) ++root;
    vector<int> parent(n, -2), leaves;
    parent[root] = -1;
    vector<pair<int, int>> stack = {{root, 0}};
    int seen = 1;
    while (!stack.empty()) {
        int u = stack.back().first;
        int& index = stack.back().second;
        if (index == (int)adj[u].size()) {
            if (u != root && adj[u].size() == 1) leaves.push_back(u);
            stack.pop_back();
            continue;
        }
        int v = adj[u][index++];
        if (v == parent[u]) continue;
        if (parent[v] != -2) throw invalid_argument("augmentTreeNoBridges: có chu trình/cạnh lặp");
        parent[v] = u;
        ++seen;
        stack.push_back({v, 0});
    }
    if (seen != n) throw invalid_argument("augmentTreeNoBridges: không liên thông");
    int original = (int)leaves.size();
    if (original & 1) leaves.push_back(leaves[0]);
    int half = (int)leaves.size() / 2;
    vector<pair<int, int>> answer;
    for (int i = 0; i < half; ++i) answer.push_back({leaves[i], leaves[i + half]});
    return answer;
}

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ tree-augmentation.hpp -o demo && ./demo
int main() {
    vector<vector<int>> tree(6);
    for (auto [u, v] : vector<pair<int, int>>{{0,1},{1,2},{1,3},{3,4},{3,5}}) {
        tree[u].push_back(v); tree[v].push_back(u);
    }
    auto extra = augmentTreeNoBridges(tree);
    printf("cần thêm %zu cạnh:", extra.size());  // 2 vì có 4 lá
    for (auto [u, v] : extra) printf(" (%d,%d)", u, v);
    printf("\n");
    return 0;
}
#endif
