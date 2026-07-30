#pragma once
#include <bits/stdc++.h>
using namespace std;
// Weighted DSU — Union-Find có thế, duy trì ràng buộc potential[v]-potential[u]=delta.
// Khi dùng: phương trình hiệu, tọa độ tương đối; thêm ràng buộc online và hỏi hiệu/kiểm mâu thuẫn.
// ĐPT: add/diff gần O(alpha(n)) amortized; bộ nhớ O(n). Đỉnh 0-based.
// Dùng: WeightedDSU d(n); d.addConstraint(u,v,w); auto delta=d.difference(u,v);
// Bẫy: delta và tổng thế dùng long long; false nghĩa là ràng buộc mới mâu thuẫn; không hỗ trợ xóa/rollback.
// CSES: 3294
struct WeightedDSU {
    vector<int> parent, size;
    vector<long long> weight;  // potential[x] - potential[parent[x]]

    // Ý nghĩa: tạo n tập đơn, thế mỗi đỉnh bằng 0 tương đối với chính nó.
    // Tham số: n = số phần tử không âm.
    explicit WeightedDSU(int n) : parent(n), size(n, 1), weight(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    // Ý nghĩa: tìm gốc và nén đường; cập nhật weight[x] thành potential[x]-potential[root].
    // Tham số: x = đỉnh 0-based.
    // Trả về: gốc đại diện.
    int find(int x) {
        if (parent[x] == x) return x;
        int oldParent = parent[x];
        parent[x] = find(oldParent);
        weight[x] += weight[oldParent];
        return parent[x];
    }

    // Ý nghĩa: thêm phương trình potential[v]-potential[u]=delta hoặc kiểm nó với thành phần hiện có.
    // Tham số: u,v = hai đỉnh; delta = hiệu thế yêu cầu.
    // Trả về: true nếu hệ vẫn nhất quán; false nếu phương trình mâu thuẫn.
    bool addConstraint(int u, int v, long long delta) {
        int ru = find(u), rv = find(v);
        long long wu = weight[u], wv = weight[v];
        if (ru == rv) return wv - wu == delta;
        if (size[ru] < size[rv]) {
            // parent[ru]=rv; cần potential[ru]-potential[rv] = wv-wu-delta.
            parent[ru] = rv;
            weight[ru] = wv - wu - delta;
            size[rv] += size[ru];
        } else {
            // parent[rv]=ru; cần potential[rv]-potential[ru] = delta+wu-wv.
            parent[rv] = ru;
            weight[rv] = delta + wu - wv;
            size[ru] += size[rv];
        }
        return true;
    }

    // Ý nghĩa: hỏi potential[v]-potential[u] nếu hai đỉnh cùng thành phần.
    // Tham số: u,v = hai đỉnh.
    // Trả về: hiệu thế; nullopt nếu chưa có quan hệ nối hai đỉnh.
    optional<long long> difference(int u, int v) {
        if (find(u) != find(v)) return nullopt;
        return weight[v] - weight[u];
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ weighted-dsu.hpp -o demo && ./demo
int main() {
    WeightedDSU d(4);
    d.addConstraint(0, 1, 5);
    d.addConstraint(1, 2, -2);
    auto value = d.difference(0, 2);
    bool consistent = d.addConstraint(0, 2, 4);
    printf("potential[2]-potential[0]=%lld; thêm delta=4 hợp lệ? %d\n",
           value.value(), (int)consistent);  // 3, 0
    return 0;
}
#endif
