#pragma once
#include <bits/stdc++.h>
using namespace std;
// Binary Lifting tổng quát — nhảy trên hàm next; hoặc LCA + gộp monoid cạnh trên đường đi cây.
// Khi dùng: lặp hàm/next-greater nhiều bước; query tổ tiên; min/max/sum/xor cạnh trên path cây tĩnh.
// ĐPT: build O(n log K), jump/path O(log K); bộ nhớ O(n log K). Đỉnh/state 0-based.
// Dùng: BinaryJump j(next); j.jump(v,k); j.jumpWhile(v,pred); TreeLift<ll,Max> t(adj,Max{},LLONG_MIN);
// Bẫy: next phải nằm trong [0,n); pathAggregate gộp CẠNH; op/id phải là monoid, thứ tự path vẫn được giữ.
// CSES: 1135 1136 1191 1664 1687 1688 1705 1750 2101 3114 3149 3304 3409

struct BinaryJump {
    int n, LOG;
    long long maxSteps;
    vector<vector<int>> up;
    // Ý nghĩa: dựng bảng up[k][v] = next^(2^k)(v).
    // Tham số: next = hàm chuyển trạng thái khép kín; maxSteps = số bước lớn nhất cần hỗ trợ.
    explicit BinaryJump(const vector<int>& next, long long maxSteps_ = 1000000000000000000LL)
        : n((int)next.size()), LOG(1), maxSteps(maxSteps_) {
        assert(maxSteps >= 0);
        while (LOG < 63 && (1ULL << LOG) <= (unsigned long long)maxSteps) ++LOG;
        up.assign(LOG, vector<int>(n));
        if (n == 0) return;
        for (int v = 0; v < n; ++v) {
            assert(0 <= next[v] && next[v] < n);
            up[0][v] = next[v];
        }
        for (int k = 1; k < LOG; ++k)
            for (int v = 0; v < n; ++v)
                up[k][v] = up[k - 1][up[k - 1][v]];
    }
    // Ý nghĩa: áp hàm next đúng steps lần lên state v.
    // Tham số: v = state đầu; steps = số bước không âm và nằm trong maxSteps đã dựng.
    // Trả về: state sau đúng steps lần chuyển.
    int jump(int v, long long steps) const {
        assert(0 <= v && v < n && 0 <= steps && steps <= maxSteps);
        for (int bit = 0; bit < LOG; ++bit)
            if ((unsigned long long)steps >> bit & 1ULL) v = up[bit][v];
        return v;
    }
    // Ý nghĩa: nhảy nhiều nhất limit bước sao cho state hạ cánh vẫn thỏa pred.
    // Tham số: v = state đầu; pred phải prefix-closed dọc quỹ đạo; limit = trần số bước.
    // Trả về: {state cuối, số bước lớn nhất}; không yêu cầu pred(v), chỉ xét state sau >=1 bước.
    template<class Predicate>
    pair<int, long long> jumpWhile(int v, Predicate pred,
                                   long long limit = LLONG_MAX) const {
        assert(0 <= v && v < n && limit >= 0);
        unsigned long long used = 0;
        unsigned long long cap = (unsigned long long)min(limit, maxSteps);
        for (int bit = LOG - 1; bit >= 0; --bit) {
            unsigned long long take = 1ULL << bit;
            if (take > cap - used) continue;
            int candidate = up[bit][v];
            if (pred(candidate)) {
                v = candidate;
                used += take;
            }
        }
        return {v, (long long)used};
    }
};

template<class T, class Combine>
struct TreeLift {
    int n, LOG;
    Combine op;
    T id;
    vector<int> depth;
    vector<vector<int>> up;
    vector<vector<T>> upward, downward;

    // Ý nghĩa: root cây bằng BFS rồi dựng tổ tiên và aggregate cạnh theo cả hai chiều.
    // Tham số: adj[u] chứa {v, giá trị cạnh uv}; combine/id tạo monoid; root = gốc.
    TreeLift(const vector<vector<pair<int, T>>>& adj, Combine combine, T identity,
             int root = 0)
        : n((int)adj.size()), LOG(1), op(std::move(combine)), id(std::move(identity)),
          depth(n, 0) {
        assert(n > 0 && 0 <= root && root < n);
        while ((1LL << LOG) <= n) ++LOG;
        up.assign(LOG, vector<int>(n, root));
        upward.assign(LOG, vector<T>(n, id));
        downward.assign(LOG, vector<T>(n, id));
        vector<char> seen(n, false);
        queue<int> queue;
        queue.push(root);
        seen[root] = true;
        int visited = 0;
        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();
            ++visited;
            for (const auto& edge : adj[u]) {
                int v = edge.first;
                if (seen[v]) continue;
                seen[v] = true;
                depth[v] = depth[u] + 1;
                up[0][v] = u;
                upward[0][v] = downward[0][v] = edge.second;
                queue.push(v);
            }
        }
        assert(visited == n);
        for (int bit = 1; bit < LOG; ++bit)
            for (int v = 0; v < n; ++v) {
                int middle = up[bit - 1][v];
                up[bit][v] = up[bit - 1][middle];
                upward[bit][v] = op(upward[bit - 1][v], upward[bit - 1][middle]);
                downward[bit][v] = op(downward[bit - 1][middle], downward[bit - 1][v]);
            }
    }
    // Ý nghĩa: leo steps cạnh từ v và gộp các cạnh đúng thứ tự v -> ancestor.
    // Tham số: v = đỉnh đầu; steps <= depth[v].
    // Trả về: {ancestor, aggregate cạnh từ v lên ancestor}; id nếu steps=0.
    pair<int, T> lift(int v, int steps) const {
        assert(0 <= v && v < n && 0 <= steps && steps <= depth[v]);
        T aggregate = id;
        for (int bit = 0; bit < LOG; ++bit)
            if (steps >> bit & 1) {
                aggregate = op(aggregate, upward[bit][v]);
                v = up[bit][v];
            }
        return {v, aggregate};
    }
    // Ý nghĩa: lấy tổ tiên chung gần nhất của a và b.
    // Tham số: a, b = hai đỉnh cây.
    // Trả về: LCA(a,b).
    int lca(int a, int b) const {
        assert(0 <= a && a < n && 0 <= b && b < n);
        if (depth[a] < depth[b]) swap(a, b);
        a = lift(a, depth[a] - depth[b]).first;
        if (a == b) return a;
        for (int bit = LOG - 1; bit >= 0; --bit)
            if (up[bit][a] != up[bit][b]) {
                a = up[bit][a];
                b = up[bit][b];
            }
        return up[0][a];
    }
    // Ý nghĩa: gộp giá trị các cạnh trên đường có hướng a -> b, đúng cả với monoid không giao hoán.
    // Tham số: a, b = hai đầu đường đi.
    // Trả về: aggregate cạnh theo thứ tự từ a tới b; id nếu a==b.
    T pathAggregate(int a, int b) const {
        int ancestor = lca(a, b);
        T left = lift(a, depth[a] - depth[ancestor]).second;
        T right = id;
        int steps = depth[b] - depth[ancestor];
        for (int bit = 0; bit < LOG; ++bit)
            if (steps >> bit & 1) {
                right = op(downward[bit][b], right);
                b = up[bit][b];
            }
        return op(left, right);
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ binary-lifting.hpp -o demo && ./demo
int main() {
    BinaryJump jump({1, 2, 3, 3});
    auto [state, steps] = jump.jumpWhile(0, [](int v) { return v <= 2; });
    vector<vector<pair<int, long long>>> tree(4);
    auto edge = [&](int u, int v, long long w) {
        tree[u].push_back({v, w}); tree[v].push_back({u, w});
    };
    edge(0, 1, 5); edge(1, 2, 2); edge(1, 3, 7);
    TreeLift<long long, function<long long(long long, long long)>> lift(
        tree, [](long long a, long long b) { return max(a, b); }, LLONG_MIN);
    printf("jump=%d/%lld; max cạnh path 2-3=%lld\n",
           state, steps, lift.pathAggregate(2, 3));  // 2/2; 7
    return 0;
}
#endif
