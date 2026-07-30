#pragma once
#include <bits/stdc++.h>
using namespace std;

// TREE RECONSTRUCTION — dựng cây nhị phân từ preorder + inorder, rồi sinh postorder.
// Khi dùng: biết thứ tự gốc-trái-phải và trái-gốc-phải của các nhãn phân biệt.
// ĐPT: O(n) kỳ vọng (kiểm tra nhãn bằng unordered_set + stack); bộ nhớ O(n).
// Dùng: auto t = reconstructBinaryTree(pre,in); auto post = t.postorderValues();
//       auto post = postorderFromPreIn(pre,in); // optional, nullopt nếu không hợp lệ
// Biểu diễn: node i chính là phần tử preorder[i]; left/right chứa chỉ số node, -1 nếu rỗng.
// Bẫy: nhãn phải PHÂN BIỆT; triển khai hoàn toàn lặp để an toàn với cây lệch n đỉnh.
// CSES: 1702
struct ReconstructedBinaryTree {
    bool valid = false;
    int root = -1;
    vector<int> value;
    vector<int> left, right;

    // Ý nghĩa: sinh postorder dưới dạng chỉ số node (trái-phải-gốc), không đệ quy.
    // Trả về: rỗng nếu cây không hợp lệ hoặc là cây rỗng.
    vector<int> postorderNodes() const {
        if (!valid || root == -1) return {};
        vector<int> stk = {root}, order;
        order.reserve(value.size());
        while (!stk.empty()) {
            int u = stk.back(); stk.pop_back();
            order.push_back(u);             // tạm thời root-right-left
            if (left[u] != -1) stk.push_back(left[u]);
            if (right[u] != -1) stk.push_back(right[u]);
        }
        reverse(order.begin(), order.end()); // left-right-root
        return order;
    }

    // Ý nghĩa: sinh postorder dưới dạng các nhãn gốc.
    vector<int> postorderValues() const {
        vector<int> result;
        for (int u : postorderNodes()) result.push_back(value[u]);
        return result;
    }
};

// Ý nghĩa: dựng duy nhất cây nhị phân tương ứng với preorder và inorder.
// Tham số: hai dãy nhãn int phân biệt; chúng phải có cùng tập phần tử.
// Trả về: cây có valid=true nếu hai traversal tương thích; ngược lại valid=false.
ReconstructedBinaryTree reconstructBinaryTree(const vector<int>& preorder,
                                               const vector<int>& inorder) {
    ReconstructedBinaryTree tree;
    int n = (int)preorder.size();
    if ((int)inorder.size() != n) return tree;
    if (n == 0) {
        tree.valid = true;
        return tree;
    }

    // Kiểm tra điều kiện nhãn phân biệt và hai dãy có cùng tập nhãn.
    unordered_set<int> remaining;
    remaining.reserve(2 * (size_t)n + 1);
    for (int x : preorder) if (!remaining.insert(x).second) return tree;
    for (int x : inorder) if (remaining.erase(x) != 1) return tree;
    if (!remaining.empty()) return tree;

    tree.value = preorder;
    tree.left.assign(n, -1);
    tree.right.assign(n, -1);
    tree.root = 0;

    vector<int> stk = {0};
    int inPos = 0;
    for (int node = 1; node < n; ++node) {
        if (inPos >= n || stk.empty()) return ReconstructedBinaryTree{};
        if (tree.value[stk.back()] != inorder[inPos]) {
            tree.left[stk.back()] = node;       // còn đang đi xuống nhánh trái
            stk.push_back(node);
        } else {
            int last = -1;
            while (!stk.empty() && inPos < n &&
                   tree.value[stk.back()] == inorder[inPos]) {
                last = stk.back();
                stk.pop_back();
                ++inPos;
            }
            if (last == -1) return ReconstructedBinaryTree{};
            tree.right[last] = node;            // nhánh trái của last đã hoàn tất
            stk.push_back(node);
        }
    }
    while (!stk.empty() && inPos < n && tree.value[stk.back()] == inorder[inPos]) {
        stk.pop_back();
        ++inPos;
    }
    if (!stk.empty() || inPos != n) return ReconstructedBinaryTree{};
    tree.valid = true;
    return tree;
}

// Ý nghĩa: chuyển trực tiếp preorder + inorder thành postorder.
// Trả về: optional chứa postorder (kể cả vector rỗng cho cây rỗng), hoặc nullopt
//         nếu dữ liệu không mô tả một cây nhị phân nhãn phân biệt hợp lệ.
optional<vector<int>> postorderFromPreIn(const vector<int>& preorder,
                                         const vector<int>& inorder) {
    ReconstructedBinaryTree tree = reconstructBinaryTree(preorder, inorder);
    if (!tree.valid) return nullopt;
    return tree.postorderValues();
}

#ifdef CP_DEMO  // g++ -std=c++17 -O2 -DCP_DEMO -x c++ tree-reconstruction.hpp -o demo && ./demo
int main() {
    vector<int> preorder = {1, 2, 4, 5, 3, 6};
    vector<int> inorder  = {4, 2, 5, 1, 3, 6};
    auto tree = reconstructBinaryTree(preorder, inorder);
    assert(tree.valid);
    assert(tree.value[tree.left[tree.root]] == 2);
    assert(tree.value[tree.right[tree.root]] == 3);
    auto post = tree.postorderValues();
    assert((post == vector<int>{4, 5, 2, 6, 3, 1}));
    assert(!postorderFromPreIn({1, 2}, {2, 2}).has_value());
    cout << "postorder:";
    for (int x : post) cout << ' ' << x;
    cout << '\n';
}
#endif
