#pragma once
#include <bits/stdc++.h>
using namespace std;

// TREE ISOMORPHISM — AHU cho cây có gốc; tâm cây + AHU cho cây không gốc.
// Khi dùng: so sánh hình dạng hai cây khi bỏ qua nhãn và thứ tự các con.
// ĐPT: O(n log n) mỗi lần mã hóa (tổng chi phí sort các danh sách con); bộ nhớ O(n).
// Dùng: rootedTreeIsomorphic(a,ra,b,rb); unrootedTreeIsomorphic(a,b);
//       AHUTreeCanonicalizer ids; int shape = ids.rootedId(tree, root);
// Chính xác: khóa vector<int> được so sánh đầy đủ bằng std::map, KHÔNG dùng hash/xác suất.
// Bẫy: adj phải là danh sách kề của một cây vô hướng 0-based. ID chỉ được so sánh
//       khi sinh bởi CÙNG một AHUTreeCanonicalizer; hai hàm so sánh đã bảo đảm điều này.
// CSES: 1700 1701
class AHUTreeCanonicalizer {
    map<vector<int>, int> idOf_;
    int nextId_ = 1;                  // 0 dành cho "chưa gán"

    int intern(vector<int> key) {
        sort(key.begin(), key.end());
        auto [it, inserted] = idOf_.emplace(std::move(key), nextId_);
        if (inserted) ++nextId_;
        return it->second;
    }

public:
    // Ý nghĩa: xóa từ điển hình dạng đã gặp; ID sau reset không so được với ID cũ.
    void clear() {
        idOf_.clear();
        nextId_ = 1;
    }

    // Ý nghĩa: trả số lớp hình dạng subtree khác nhau đã được intern.
    int shapeCount() const { return nextId_ - 1; }

    // Ý nghĩa: gán ID AHU chính xác cho mọi subtree khi đặt gốc tại root.
    // Tham số: adj = cây vô hướng; root = gốc 0-based.
    // Trả về: ids[u] là ID của subtree u (theo hướng cha sinh bởi root).
    vector<int> rootedIds(const vector<vector<int>>& adj, int root) {
        int n = (int)adj.size();
        if (n == 0) return {};
        assert(0 <= root && root < n);
        vector<int> parent(n, -2), order;
        order.reserve(n);
        parent[root] = -1;
        order.push_back(root);
        for (size_t i = 0; i < order.size(); ++i) {
            int u = order[i];
            for (int v : adj[u]) if (parent[v] == -2) {
                parent[v] = u;
                order.push_back(v);
            }
        }
        assert((int)order.size() == n); // API yêu cầu adj là một cây liên thông

        vector<int> ids(n, 0);
        vector<int> childIds;
        for (int i = n - 1; i >= 0; --i) {
            int u = order[i];
            childIds.clear();
            childIds.reserve(adj[u].size());
            for (int v : adj[u]) if (parent[v] == u) childIds.push_back(ids[v]);
            ids[u] = intern(childIds);
        }
        return ids;
    }

    // Ý nghĩa: lấy ID AHU của toàn cây có gốc.
    // Trả về: 0 cho cây rỗng; ngược lại là ID chính xác của hình dạng tại root.
    int rootedId(const vector<vector<int>>& adj, int root) {
        if (adj.empty()) return 0;
        return rootedIds(adj, root)[root];
    }
};

// Ý nghĩa: tìm một hoặc hai tâm của cây bằng bóc lá theo lớp.
// Trả về: vector tăng dần gồm 1 tâm (đường kính chẵn) hoặc 2 tâm (đường kính lẻ);
//         cây rỗng trả vector rỗng.
vector<int> treeCenters(const vector<vector<int>>& adj) {
    int n = (int)adj.size();
    if (n == 0) return {};
    vector<int> degree(n);
    queue<int> leaves;
    for (int u = 0; u < n; ++u) {
        degree[u] = (int)adj[u].size();
        if (degree[u] <= 1) leaves.push(u);
    }
    int remaining = n;
    while (remaining > 2) {
        int layer = (int)leaves.size();
        assert(layer > 0);             // một cây hợp lệ luôn còn lá
        remaining -= layer;
        while (layer--) {
            int u = leaves.front(); leaves.pop();
            for (int v : adj[u]) if (--degree[v] == 1) leaves.push(v);
        }
    }
    vector<int> centers;
    while (!leaves.empty()) {
        centers.push_back(leaves.front());
        leaves.pop();
    }
    sort(centers.begin(), centers.end());
    return centers;
}

// Ý nghĩa: so sánh đẳng cấu hai cây CÓ GỐC bằng một từ điển AHU dùng chung.
// Trả về: true khi tồn tại song ánh bảo toàn cạnh và ánh rootA sang rootB.
bool rootedTreeIsomorphic(const vector<vector<int>>& a, int rootA,
                          const vector<vector<int>>& b, int rootB) {
    if (a.size() != b.size()) return false;
    if (a.empty()) return true;
    AHUTreeCanonicalizer canonicalizer;
    return canonicalizer.rootedId(a, rootA) == canonicalizer.rootedId(b, rootB);
}

// Ý nghĩa: so sánh đẳng cấu hai cây KHÔNG GỐC.
// Cách làm: tâm là bất biến đẳng cấu; cố định một tâm của a và thử tối đa hai tâm của b.
bool unrootedTreeIsomorphic(const vector<vector<int>>& a,
                            const vector<vector<int>>& b) {
    if (a.size() != b.size()) return false;
    if (a.empty()) return true;
    vector<int> ca = treeCenters(a), cb = treeCenters(b);
    if (ca.size() != cb.size()) return false;
    AHUTreeCanonicalizer canonicalizer;
    int idA = canonicalizer.rootedId(a, ca[0]);
    for (int rootB : cb)
        if (canonicalizer.rootedId(b, rootB) == idA) return true;
    return false;
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ tree-isomorphism.hpp -o demo && ./demo
int main() {
    auto makeTree = [](int n, initializer_list<pair<int, int>> edges) {
        vector<vector<int>> g(n);
        for (auto [u, v] : edges) { g[u].push_back(v); g[v].push_back(u); }
        return g;
    };
    auto a = makeTree(5, {{0,1},{0,2},{2,3},{2,4}});
    auto b = makeTree(5, {{4,1},{4,3},{3,0},{3,2}}); // cùng hình dạng, đổi nhãn
    auto c = makeTree(5, {{0,1},{1,2},{2,3},{3,4}}); // đường đi
    assert(rootedTreeIsomorphic(a, 0, b, 4));
    assert(unrootedTreeIsomorphic(a, b));
    assert(!unrootedTreeIsomorphic(a, c));
    assert((treeCenters(c) == vector<int>{2}));
    cout << "a và b đẳng cấu; số tâm của a = " << treeCenters(a).size() << '\n';
}
#endif
