#pragma once
#include <bits/stdc++.h>
using namespace std;

// BLOCK-CUT FOREST — thành phần song liên thông theo ĐỈNH (vertex-BCC) bằng Tarjan low-link.
// Khi dùng: khớp, các "khối" không bị tách bởi một đỉnh, truy vấn đường đi tránh một đỉnh.
// ĐPT: dựng O(n + m + (n + B) log(n + B)), truy vấn O(log(n + B)); B là số block.
// Dùng: BlockCutForest b(n); b.addEdge(u,v); b.build();
//       b.blocks[k], b.forest, b.isArticulation[v], b.canAvoidVertex(a,b,c).
// Biểu diễn: node 0..n-1 của forest là đỉnh gốc; node n+k là block blocks[k].
// Bẫy: đồ thị VÔ HƯỚNG, addEdge đúng một lần/cạnh; hỗ trợ cạnh song song.
//       Self-loop không ảnh hưởng liên thông/khớp nên bị bỏ qua trong phân rã.
// CSES: 1705
struct BlockCutForest {
    int n, m = 0;
    vector<vector<pair<int, int>>> adj; // (đỉnh kề, id cạnh)
    vector<pair<int, int>> edges;

    vector<vector<int>> blocks;         // danh sách đỉnh gốc thuộc từng vertex-BCC
    vector<char> isArticulation;
    vector<vector<int>> forest;         // block-vertex forest, kích thước n + blocks.size()
    vector<int> depth, component;

private:
    vector<vector<int>> up_;
    int log_ = 0;

    // Ý nghĩa: LCA trên block-cut forest; chỉ gọi khi u, v cùng component.
    int lcaSameComponent(int u, int v) const {
        if (depth[u] < depth[v]) swap(u, v);
        int diff = depth[u] - depth[v];
        for (int k = 0; k < log_; ++k) if (diff >> k & 1) u = up_[k][u];
        if (u == v) return u;
        for (int k = log_ - 1; k >= 0; --k) if (up_[k][u] != up_[k][v]) {
            u = up_[k][u];
            v = up_[k][v];
        }
        return up_[0][u];
    }

public:
    // Ý nghĩa: khởi tạo đồ thị vô hướng n đỉnh, chưa có cạnh.
    explicit BlockCutForest(int n) : n(n), adj(n), isArticulation(n, 0) {}

    // Ý nghĩa: thêm một cạnh vô hướng u-v và gán id cạnh duy nhất.
    // Tham số: u, v = hai đầu mút 0-based.
    void addEdge(int u, int v) {
        edges.push_back({u, v});
        adj[u].push_back({v, m});
        adj[v].push_back({u, m});
        ++m;
    }

    // Ý nghĩa: phân rã vertex-BCC, dựng block-cut forest và bảng LCA.
    // Trả về: số block; đỉnh cô lập cũng nhận một singleton block.
    int build() {
        blocks.clear();
        fill(isArticulation.begin(), isArticulation.end(), 0);
        vector<int> disc(n, -1), low(n, -1), parent(n, -1), parentEdge(n, -1);
        vector<int> nextEdge(n, 0), childCount(n, 0), edgeStack, dfsStack;
        vector<int> seen(n, 0);
        int timer = 0, stamp = 0;

        auto extractBlock = [&](int stopEdge) {
            vector<int> vertices;
            ++stamp;
            while (!edgeStack.empty()) {
                int eid = edgeStack.back();
                edgeStack.pop_back();
                auto [a, b] = edges[eid];
                if (seen[a] != stamp) { seen[a] = stamp; vertices.push_back(a); }
                if (seen[b] != stamp) { seen[b] = stamp; vertices.push_back(b); }
                if (eid == stopEdge) break;
            }
            sort(vertices.begin(), vertices.end());
            if (!vertices.empty()) blocks.push_back(std::move(vertices));
        };

        // DFS low-link lặp; stack cạnh chứa đúng các cạnh của các block chưa đóng.
        for (int root = 0; root < n; ++root) if (disc[root] == -1) {
            disc[root] = low[root] = timer++;
            dfsStack.push_back(root);
            while (!dfsStack.empty()) {
                int u = dfsStack.back();
                if (nextEdge[u] < (int)adj[u].size()) {
                    auto [v, eid] = adj[u][nextEdge[u]++];
                    if (u == v) continue;                     // self-loop không đổi kết quả
                    if (eid == parentEdge[u]) continue;       // bỏ đúng cạnh cây đi vào
                    if (disc[v] == -1) {
                        parent[v] = u;
                        parentEdge[v] = eid;
                        ++childCount[u];
                        edgeStack.push_back(eid);
                        disc[v] = low[v] = timer++;
                        dfsStack.push_back(v);
                    } else if (disc[v] < disc[u]) {            // back-edge lên tổ tiên
                        edgeStack.push_back(eid);
                        low[u] = min(low[u], disc[v]);
                    }
                } else {
                    dfsStack.pop_back();
                    int p = parent[u];
                    if (p == -1) {
                        if (childCount[u] > 1) isArticulation[u] = 1;
                    } else {
                        low[p] = min(low[p], low[u]);
                        if (low[u] >= disc[p]) {
                            if (parent[p] != -1) isArticulation[p] = 1;
                            extractBlock(parentEdge[u]);
                        }
                    }
                }
            }
        }

        // Đỉnh không nằm trên cạnh thường nào (cô lập/chỉ có self-loop) là một block đơn.
        vector<int> incidence(n, 0);
        for (const auto &block : blocks) for (int v : block) ++incidence[v];
        for (int v = 0; v < n; ++v) if (incidence[v] == 0) blocks.push_back({v});

        forest.assign(n + (int)blocks.size(), {});
        for (int b = 0; b < (int)blocks.size(); ++b) {
            int blockNode = n + b;
            for (int v : blocks[b]) {
                forest[v].push_back(blockNode);
                forest[blockNode].push_back(v);
            }
        }

        // Tiền xử lý binary lifting trên từng cây của forest.
        int total = (int)forest.size();
        log_ = 1;
        while ((1LL << log_) <= max(1, total)) ++log_;
        up_.assign(log_, vector<int>(total, 0));
        depth.assign(total, 0);
        component.assign(total, -1);
        queue<int> q;
        for (int root = 0; root < total; ++root) if (component[root] == -1) {
            component[root] = root;
            up_[0][root] = root;
            q.push(root);
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : forest[u]) if (component[v] == -1) {
                    component[v] = root;
                    depth[v] = depth[u] + 1;
                    up_[0][v] = u;
                    q.push(v);
                }
            }
        }
        for (int k = 1; k < log_; ++k)
            for (int v = 0; v < total; ++v)
                up_[k][v] = up_[k - 1][up_[k - 1][v]];
        return (int)blocks.size();
    }

    // Ý nghĩa: đổi chỉ số block k sang chỉ số node tương ứng trong forest.
    int blockNode(int k) const { return n + k; }

    // Ý nghĩa: khoảng cách giữa hai node bất kỳ của block-cut forest.
    // Trả về: số cạnh trên đường đi, hoặc -1 nếu khác thành phần.
    int forestDistance(int u, int v) const {
        if (u < 0 || v < 0 || u >= (int)forest.size() || v >= (int)forest.size())
            return -1;
        if (component[u] != component[v]) return -1;
        int w = lcaSameComponent(u, v);
        return depth[u] + depth[v] - 2 * depth[w];
    }

    // Ý nghĩa: kiểm tra node x có nằm trên đường duy nhất a-b trong block-cut forest.
    bool onForestPath(int x, int a, int b) const {
        int ab = forestDistance(a, b);
        if (ab < 0) return false;
        int ax = forestDistance(a, x), xb = forestDistance(x, b);
        return ax >= 0 && xb >= 0 && ax + xb == ab;
    }

    // Ý nghĩa: kiểm tra bỏ đỉnh gốc cut có làm a và b mất liên thông không.
    // Trả về: true cả khi cut == a hoặc cut == b; false nếu a,b vốn khác component.
    bool separatesVertex(int cut, int a, int b) const {
        if (cut < 0 || cut >= n || a < 0 || a >= n || b < 0 || b >= n) return false;
        if (component[a] != component[b] || component[cut] != component[a]) return false;
        return onForestPath(cut, a, b);
    }

    // Ý nghĩa: có tồn tại đường a -> b trong đồ thị sau khi cấm đỉnh forbidden không.
    // Trả về: false nếu a,b vốn không liên thông hoặc forbidden là một đầu mút.
    bool canAvoidVertex(int a, int b, int forbidden) const {
        if (a < 0 || a >= n || b < 0 || b >= n || forbidden < 0 || forbidden >= n)
            return false;
        if (component[a] != component[b] || forbidden == a || forbidden == b) return false;
        return !separatesVertex(forbidden, a, b);
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ block-cut-tree.hpp -o demo && ./demo
int main() {
    BlockCutForest g(6);
    g.addEdge(0, 1); g.addEdge(1, 2); g.addEdge(2, 0); // block tam giác
    g.addEdge(2, 3);                                   // cầu
    g.addEdge(3, 4); g.addEdge(4, 5); g.addEdge(5, 3); // block tam giác
    g.build();
    assert(g.isArticulation[2] && g.isArticulation[3]);
    assert(!g.canAvoidVertex(0, 5, 2));
    assert(g.canAvoidVertex(0, 1, 2));                  // còn cạnh trực tiếp 0-1
    assert(!g.canAvoidVertex(0, 5, 0));                 // cấm đầu mút
    cout << "số block = " << g.blocks.size()
         << ", tránh đỉnh 2 trên đường 0-5: NO\n";
}
#endif
