#pragma once
#include <bits/stdc++.h>
using namespace std;
// DSU (Union-Find) — connectivity; kèm ParityDSU cho ràng buộc XOR/chẵn-lẻ giữa hai đỉnh.
// Khi dùng: connectivity chỉ THÊM cạnh, Kruskal MST, gộp nhóm offline, kiểm tra đồ thị hai phía động.
// ĐPT: find/unite ~O(α(n)) khấu hao; bộ nhớ O(n). Index 0-based.
// Dùng: DSU d(n); d.unite(a,b); d.same(a,b); d.size(a); d.comp (số thành phần).
// Bẫy: KHÔNG hỗ trợ tách/xóa cạnh — cần undo thì dùng rollback-dsu; API kiểm tra index.
// CSES: 1136 1666 1675 1676 1677 1699 1706 2078 2101 2402 3111 3407 3408 3409
struct DSU {
    vector<int> p, sz;
    int comp;
    // Ý nghĩa: khởi tạo n phần tử rời nhau, mỗi phần tử là gốc của riêng nó (comp = n).
    // Tham số: n = số phần tử (index 0-based).
    explicit DSU(int n) : p(max(0, n)), sz(max(0, n), 1), comp(n) {
        if (n < 0) throw invalid_argument("DSU: n âm");
        iota(p.begin(), p.end(), 0);
    }
    // Ý nghĩa: tìm đại diện (gốc) của tập chứa x, có nén đường đi (path halving).
    // Tham số: x = phần tử cần tra cứu.
    // Trả về: chỉ số phần tử gốc của thành phần chứa x.
    int find(int x) {
        if (x < 0 || x >= (int)p.size()) throw out_of_range("DSU::find: đỉnh ngoài miền");
        while (x != p[x]) x = p[x] = p[p[x]];
        return x;
    }  // nén đường
    // Ý nghĩa: hợp nhất hai tập chứa a và b theo union by size, giảm comp nếu gộp thật.
    // Tham số: a, b = hai phần tử cần gộp.
    // Trả về: true nếu a và b khác tập và đã được gộp; false nếu vốn đã cùng tập.
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; comp--;
        return true;
    }
    // Ý nghĩa: kiểm tra a và b có thuộc cùng một thành phần hay không.
    // Tham số: a, b = hai phần tử cần so sánh.
    // Trả về: true nếu cùng tập; false nếu khác tập.
    bool same(int a, int b) { return find(a) == find(b); }
    // Ý nghĩa: lấy kích thước (số phần tử) của thành phần chứa x.
    // Tham số: x = phần tử bất kỳ trong thành phần.
    // Trả về: số phần tử của thành phần chứa x.
    int size(int x) { return sz[find(x)]; }
};

// DSU có trọng số XOR 1 bit: lưu parity[x] XOR parity[parent[x]].
// Dùng: ParityDSU d(n); d.addConstraint(a,b,1); // a và b phải khác màu
// Bẫy: false từ addConstraint nghĩa là ràng buộc mới mâu thuẫn với các ràng buộc trước.
struct ParityDSU {
    vector<int> p, sz, xorToParent;
    int comp;

    // Ý nghĩa: tạo n đỉnh độc lập, chưa có quan hệ parity giữa các đỉnh khác nhau.
    // Tham số: n = số đỉnh, index 0-based.
    explicit ParityDSU(int n)
        : p(max(0, n)), sz(max(0, n), 1), xorToParent(max(0, n), 0), comp(n) {
        if (n < 0) throw invalid_argument("ParityDSU: n âm");
        iota(p.begin(), p.end(), 0);
    }

    // Ý nghĩa: tìm gốc và XOR từ x đến gốc, đồng thời nén đường đi.
    // Tham số: x = đỉnh hợp lệ.
    // Trả về: {root, parity[x] XOR parity[root]}.
    pair<int, int> findWithParity(int x) {
        checkVertex(x);
        if (p[x] == x) return {x, 0};
        int parent = p[x];
        auto [root, parentToRoot] = findWithParity(parent);
        xorToParent[x] ^= parentToRoot;
        p[x] = root;
        return {root, xorToParent[x]};
    }

    // Ý nghĩa: tìm đại diện của thành phần chứa x.
    // Tham số: x = đỉnh hợp lệ.
    // Trả về: gốc của thành phần.
    int find(int x) { return findWithParity(x).first; }

    // Ý nghĩa: thêm ràng buộc parity[a] XOR parity[b] = requiredParity.
    // Tham số: a,b = hai đỉnh; requiredParity phải là 0 (giống) hoặc 1 (khác).
    // Trả về: true nếu ràng buộc nhất quán; false nếu nó tạo mâu thuẫn.
    bool addConstraint(int a, int b, int requiredParity) {
        if (requiredParity != 0 && requiredParity != 1)
            throw invalid_argument("ParityDSU: parity phải là 0/1");
        auto [rootA, parityA] = findWithParity(a);
        auto [rootB, parityB] = findWithParity(b);
        if (rootA == rootB)
            return (parityA ^ parityB) == requiredParity;

        if (sz[rootA] < sz[rootB]) swap(rootA, rootB);
        p[rootB] = rootA;
        xorToParent[rootB] = parityA ^ parityB ^ requiredParity;
        sz[rootA] += sz[rootB];
        --comp;
        return true;
    }

    // Ý nghĩa: hỏi quan hệ parity giữa hai đỉnh nếu chúng đã liên thông.
    // Tham số: a,b = hai đỉnh.
    // Trả về: nullopt nếu khác thành phần; ngược lại là parity[a] XOR parity[b].
    optional<int> difference(int a, int b) {
        auto [rootA, parityA] = findWithParity(a);
        auto [rootB, parityB] = findWithParity(b);
        if (rootA != rootB) return nullopt;
        return parityA ^ parityB;
    }

    // Ý nghĩa: kiểm tra hai đỉnh đã có quan hệ xác định hay chưa.
    // Tham số: a,b = hai đỉnh.
    // Trả về: true khi cùng thành phần.
    bool same(int a, int b) { return find(a) == find(b); }

    // Ý nghĩa: lấy số đỉnh trong thành phần chứa x.
    // Tham số: x = đỉnh hợp lệ.
    // Trả về: kích thước thành phần.
    int size(int x) { return sz[find(x)]; }

private:
    void checkVertex(int x) const {
        if (x < 0 || x >= (int)p.size())
            throw out_of_range("ParityDSU: đỉnh ngoài miền");
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ dsu.hpp -o demo && ./demo
int main() {
    DSU d(5);
    d.unite(0, 1);
    d.unite(3, 4);
    printf("same(0,1)=%d  same(0,2)=%d  comp=%d  size(0)=%d\n",
           d.same(0, 1), d.same(0, 2), d.comp, d.size(0));
    ParityDSU parity(3);
    parity.addConstraint(0, 1, 1);
    parity.addConstraint(1, 2, 1);
    printf("parity(0,2)=%d\n", *parity.difference(0, 2));  // 0
    return 0;
}
#endif
