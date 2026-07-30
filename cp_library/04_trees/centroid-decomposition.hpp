#pragma once
#include <bits/stdc++.h>
using namespace std;
// Centroid Decomposition — đếm ĐƯỜNG ĐI hoặc duy trì đỉnh active gần nhất trên CÂY.
// Khi dùng: đếm cặp theo khoảng cách; hoặc online add đỉnh active + hỏi khoảng cách tới active gần nhất.
// ĐPT: CentroidPaths O(n log²n); CentroidNearest build O(n log n), add/query O(log n); bộ nhớ O(n log n).
// Dùng: CentroidPaths p(n); p.countPaths(lo,hi); hoặc CentroidNearest q(adj); q.activate(v); q.query(v);
// Bẫy: cây không trọng số, đỉnh 0-based; CentroidNearest chỉ THÊM active (không xóa); phải là cây/liên thông.
// CSES: 1752 2079 2080 2081
struct CentroidPaths {
    int n, lo, hi;
    vector<vector<int>> adj;
    vector<int> sz, par;
    vector<char> removed;
    vector<long long> bit;   // Fenwick theo khoảng cách [0..n]
    long long answer = 0;
    // Ý nghĩa: khởi tạo cây n đỉnh (0-based) và Fenwick khoảng cách rỗng.
    // Tham số: n = số đỉnh.
    explicit CentroidPaths(int n) : n(n), lo(0), hi(0), adj(n), sz(n, 0), par(n, -1), removed(n, 0), bit(n + 2, 0) {}
    // Ý nghĩa: thêm cạnh vô hướng u - v vào cây.
    // Tham số: u, v = hai đầu mút của cạnh.
    void addEdge(int u, int v) { adj[u].push_back(v); adj[v].push_back(u); }
    // Ý nghĩa: cộng 1 lượng vào Fenwick tại khoảng cách i (0-based, tự dời sang 1-based bên trong).
    // Tham số: i = khoảng cách; v = lượng cộng thêm.
    void bitAdd(int i, long long v) { for (i++; i <= n + 1; i += i & -i) bit[i] += v; }
    // Ý nghĩa: tổng tiền tố Fenwick của các khoảng cách 0..i.
    // Tham số: i = khoảng cách biên phải (bao gồm).
    // Trả về: số đỉnh đã thêm có khoảng cách <= i.
    long long bitPre(int i) { long long s = 0; for (i++; i > 0; i -= i & -i) s += bit[i]; return s; }
    // Ý nghĩa: đếm số đỉnh đã thêm có khoảng cách nằm trong đoạn [l, r] (tự kẹp về [0, n]).
    // Tham số: l, r = biên đoạn khoảng cách (đóng).
    // Trả về: số đỉnh có khoảng cách trong [l, r], 0 nếu đoạn rỗng.
    long long bitRange(int l, int r) {
        l = max(l, 0); r = min(r, n);
        if (l > r) return 0;
        return bitPre(r) - (l > 0 ? bitPre(l - 1) : 0);
    }
    // Ý nghĩa: thu thập các đỉnh (chưa bị xóa) của thành phần chứa entry theo DFS lặp, đặt par cho từng đỉnh.
    // Tham số: entry = một đỉnh của thành phần; comp = vector nhận danh sách đỉnh (bổ sung vào cuối).
    void component(int entry, vector<int>& comp) {
        vector<int> st = {entry}; par[entry] = -1;
        while (!st.empty()) {
            int u = st.back(); st.pop_back(); comp.push_back(u);
            for (int v : adj[u]) if (v != par[u] && !removed[v]) { par[v] = u; st.push_back(v); }
        }
    }
    // Ý nghĩa: phân rã thành phần chứa entry: tìm trọng tâm, đếm cặp đi qua nó bằng Fenwick, rồi đệ quy các phần con.
    // Tham số: entry = một đỉnh của thành phần hiện tại.
    void decompose(int entry) {
        vector<int> comp;
        component(entry, comp);
        for (int i = (int)comp.size() - 1; i >= 0; i--) {   // kích thước cây con (thứ tự đảo = post-order)
            int u = comp[i]; sz[u] = 1;
            for (int v : adj[u]) if (v != par[u] && !removed[v]) sz[u] += sz[v];
        }
        int tot = (int)comp.size(), c = entry, p = -1;
        while (true) {   // đi xuống con nặng (> tot/2) để tới trọng tâm
            int nxt = -1;
            for (int v : adj[c]) if (v != p && !removed[v] && sz[v] > tot / 2) { nxt = v; break; }
            if (nxt == -1) break;
            p = c; c = nxt;
        }
        vector<int> touched = {0};
        bitAdd(0, 1);   // trọng tâm ở khoảng cách 0
        for (int w : adj[c]) if (!removed[w]) {
            vector<int> ds;
            vector<array<int, 3>> stk = {{w, c, 1}};   // (đỉnh, cha, khoảng cách tới trọng tâm)
            while (!stk.empty()) {
                auto [u, pp, d] = stk.back(); stk.pop_back();
                ds.push_back(d);
                for (int v : adj[u]) if (v != pp && !removed[v]) stk.push_back({v, u, d + 1});
            }
            for (int d : ds) answer += bitRange(lo - d, hi - d);   // ghép với đỉnh đã thêm (trọng tâm + cây con trước)
            for (int d : ds) { bitAdd(d, 1); touched.push_back(d); }
        }
        for (int d : touched) bitAdd(d, -1);   // hoàn tác Fenwick cho lần sau
        removed[c] = 1;
        for (int v : adj[c]) if (!removed[v]) decompose(v);
    }
    // Ý nghĩa: đếm số cặp đỉnh không thứ tự có khoảng cách (số cạnh) nằm trong [lo, hi].
    // Tham số: lo, hi = biên khoảng cách (đóng).
    // Trả về: số cặp thỏa điều kiện.
    long long countPaths(int lo_, int hi_) {
        lo = lo_; hi = hi_; answer = 0;
        removed.assign(n, 0);
        if (n > 0) decompose(0);
        return answer;
    }
};

struct CentroidNearest {
    static constexpr int INF = INT_MAX / 4;
    int n;
    vector<vector<int>> adj;
    vector<int> sz, parent, best;
    vector<char> removed;
    vector<vector<pair<int, int>>> ancestors;  // (centroid ancestor, khoảng cách tới centroid)
    bool built = false;

    // Ý nghĩa: khởi tạo cấu trúc nearest-active từ danh sách kề của cây và dựng centroid decomposition lặp.
    // Tham số: graph = cây vô hướng, không trọng số, đỉnh 0-based.
    explicit CentroidNearest(const vector<vector<int>>& graph)
        : n((int)graph.size()), adj(graph), sz(n), parent(n), best(n, INF),
          removed(n, false), ancestors(n) {
        build();
    }
    // Ý nghĩa: khởi tạo cây rỗng n đỉnh; gọi addEdge đủ n-1 cạnh rồi gọi build trước activate/query.
    // Tham số: n = số đỉnh.
    explicit CentroidNearest(int n)
        : n(n), adj(n), sz(n), parent(n), best(n, INF), removed(n, false),
          ancestors(n) {}
    // Ý nghĩa: thêm cạnh vô hướng trước khi build.
    // Tham số: u, v = hai đầu cạnh 0-based.
    void addEdge(int u, int v) {
        assert(!built && 0 <= u && u < n && 0 <= v && v < n);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    // Ý nghĩa: lấy toàn bộ thành phần chưa loại chứa entry, đồng thời dựng parent và subtree size tạm.
    // Tham số: entry = một đỉnh chưa bị loại; component = nơi nhận các đỉnh của thành phần.
    void collectComponent(int entry, vector<int>& component) {
        component.clear();
        vector<int> stack = {entry};
        parent[entry] = -1;
        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            component.push_back(u);
            for (int v : adj[u])
                if (!removed[v] && v != parent[u]) {
                    parent[v] = u;
                    stack.push_back(v);
                }
        }
        for (int i = (int)component.size() - 1; i >= 0; --i) {
            int u = component[i];
            sz[u] = 1;
            for (int v : adj[u])
                if (!removed[v] && parent[v] == u) sz[u] += sz[v];
        }
    }
    // Ý nghĩa: chọn centroid của component đã được root tạm bởi collectComponent.
    // Tham số: component = danh sách đỉnh của đúng một thành phần chưa bị loại.
    // Trả về: đỉnh có mọi mảnh sau khi xóa không quá nửa kích thước component.
    int chooseCentroid(const vector<int>& component) const {
        int total = (int)component.size();
        for (int u : component) {
            int largest = total - sz[u];
            for (int v : adj[u])
                if (!removed[v] && parent[v] == u) largest = max(largest, sz[v]);
            if (largest * 2 <= total) return u;
        }
        assert(false);
        return -1;
    }
    // Ý nghĩa: dựng mọi centroid ancestor và khoảng cách; toàn bộ traversal dùng stack tường minh.
    void build() {
        assert(!built);
        built = true;
        if (n == 0) return;
        vector<int> pending = {0}, component;
        int processed = 0;
        while (!pending.empty()) {
            int entry = pending.back();
            pending.pop_back();
            if (removed[entry]) continue;
            collectComponent(entry, component);
            int centroid = chooseCentroid(component);
            vector<array<int, 3>> stack = {{centroid, -1, 0}};
            while (!stack.empty()) {
                auto [u, p, distance] = stack.back();
                stack.pop_back();
                ancestors[u].push_back({centroid, distance});
                for (int v : adj[u])
                    if (v != p && !removed[v])
                        stack.push_back({v, u, distance + 1});
            }
            removed[centroid] = true;
            ++processed;
            for (int v : adj[centroid])
                if (!removed[v]) pending.push_back(v);
        }
        assert(processed == n);
    }
    // Ý nghĩa: đánh dấu v active; các lần gọi lặp hoặc thêm nhiều đỉnh đều hợp lệ.
    // Tham số: v = đỉnh cần kích hoạt.
    void activate(int v) {
        assert(built && 0 <= v && v < n);
        for (auto [centroid, distance] : ancestors[v])
            best[centroid] = min(best[centroid], distance);
    }
    // Ý nghĩa: hỏi khoảng cách ngắn nhất từ v tới một đỉnh đã activate.
    // Tham số: v = đỉnh truy vấn.
    // Trả về: số cạnh ngắn nhất, hoặc -1 nếu chưa có đỉnh active.
    int query(int v) const {
        assert(built && 0 <= v && v < n);
        int answer = INF;
        for (auto [centroid, distance] : ancestors[v])
            answer = min(answer, distance + best[centroid]);
        return answer >= INF ? -1 : answer;
    }
    // Ý nghĩa: xóa toàn bộ trạng thái active nhưng giữ decomposition để dùng lại.
    void clearActive() { fill(best.begin(), best.end(), INF); }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ centroid-decomposition.hpp -o demo && ./demo
int main() {
    // Đường thẳng 0-1-2-3-4
    CentroidPaths t(5);
    t.addEdge(0, 1); t.addEdge(1, 2); t.addEdge(2, 3); t.addEdge(3, 4);
    printf("số cặp khoảng cách =2: %lld ; khoảng cách trong [1,2]: %lld\n",
           t.countPaths(2, 2), t.countPaths(1, 2));   // 3 ; 7
    CentroidNearest nearest(5);
    nearest.addEdge(0, 1); nearest.addEdge(1, 2); nearest.addEdge(2, 3); nearest.addEdge(3, 4);
    nearest.build();
    nearest.activate(0); nearest.activate(4);
    printf("active gần 2 nhất cách %d cạnh\n", nearest.query(2));  // 2
    return 0;
}
#endif
