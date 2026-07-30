#pragma once
#include <bits/stdc++.h>
using namespace std;

// HAVEL–HAKIMI — dựng đồ thị ĐƠN vô hướng từ một dãy bậc.
// Khi dùng: ghép cặp sao cho đỉnh i có đúng degree[i] hàng xóm, mỗi cặp nhiều nhất một cạnh.
// ĐPT: O((n + m) log n), m = sum(degree)/2; bộ nhớ O(n + m).
// Dùng: auto r = havelHakimi(degree); if (r.possible) dùng r.edges (0-based).
// Bẫy: bậc phải thuộc [0,n-1], tổng bậc chẵn; kết quả là một realization bất kỳ.
// CSES: 1697
struct SimpleGraphRealization {
    bool possible = false;
    vector<pair<int, int>> edges; // các cạnh 0-based, không self-loop, không trùng
};

// Ý nghĩa: kiểm tra và dựng một đồ thị đơn có đúng dãy bậc đã cho bằng Havel–Hakimi.
// Tham số: degree[i] = bậc yêu cầu của đỉnh i, các đỉnh đánh số 0..n-1.
// Trả về: {possible=false,{}} nếu dãy không graphical; nếu hợp lệ, edges có
//         đúng sum(degree)/2 cạnh và bậc từng đỉnh đúng yêu cầu.
SimpleGraphRealization havelHakimi(const vector<int>& degree) {
    struct Node {
        int degree, vertex;
    };
    struct ByRemainingDegree {
        bool operator()(const Node& a, const Node& b) const {
            if (a.degree != b.degree) return a.degree < b.degree;
            return a.vertex > b.vertex; // hòa: lấy chỉ số nhỏ trước để kết quả ổn định
        }
    };

    const int n = (int)degree.size();
    long long sum = 0;
    priority_queue<Node, vector<Node>, ByRemainingDegree> pq;
    for (int v = 0; v < n; ++v) {
        if (degree[v] < 0 || degree[v] >= n) return {};
        sum += degree[v];
        if (degree[v] > 0) pq.push({degree[v], v});
    }
    if (sum & 1LL) return {};

    SimpleGraphRealization result;
    result.edges.reserve((size_t)(sum / 2));
    vector<Node> changed;
    while (!pq.empty()) {
        Node u = pq.top(); pq.pop();
        if (u.degree > (int)pq.size()) return {};
        changed.clear();
        changed.reserve(u.degree);
        for (int i = 0; i < u.degree; ++i) {
            Node v = pq.top(); pq.pop();
            result.edges.push_back({u.vertex, v.vertex});
            --v.degree;
            changed.push_back(v);
        }
        for (Node v : changed) if (v.degree > 0) pq.push(v);
    }
    result.possible = true;
    return result;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ havel-hakimi.hpp -o demo && ./demo
int main() {
    vector<int> degree = {2, 2, 2, 2}; // có thể dựng chu trình 4 đỉnh
    auto r = havelHakimi(degree);
    assert(r.possible && r.edges.size() == 4);
    vector<int> got(4, 0);
    set<pair<int, int>> uniqueEdges;
    for (auto [u, v] : r.edges) {
        assert(u != v);
        if (u > v) swap(u, v);
        assert(uniqueEdges.insert({u, v}).second);
        ++got[u]; ++got[v];
    }
    assert(got == degree);
    assert(!havelHakimi({3, 3, 1, 1}).possible);
    cout << "dựng được " << r.edges.size() << " cạnh\n";
}
#endif
