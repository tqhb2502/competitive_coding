// Tree Traversals (CSES 1702)
// https://cses.fi/problemset/task/1702
//
// Cho preorder + inorder của một cây nhị phân (nhãn phân biệt 1..n),
// tái tạo cây bằng stack lặp (O(n)) rồi in ra postorder bằng duyệt
// hai-stack lặp để tránh tràn stack với n = 1e5.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    vector<int> pre(n), in(n);
    for (int i = 0; i < n; i++) scanf("%d", &pre[i]);
    for (int i = 0; i < n; i++) scanf("%d", &in[i]);

    // Con trái / con phải theo nhãn đỉnh (0 = không có con).
    vector<int> lc(n + 1, 0), rc(n + 1, 0);

    // --- Bước 1: dựng cây bằng stack lặp ---
    vector<int> stk;
    stk.reserve(n);
    stk.push_back(pre[0]);
    int inIdx = 0;
    for (int i = 1; i < n; i++) {
        int cur = pre[i];
        if (stk.back() != in[inIdx]) {
            // cur là con trái của đỉnh trên cùng stack
            lc[stk.back()] = cur;
        } else {
            // đã hết nhánh trái: quay lên tìm cha của con phải
            int last = 0;
            while (!stk.empty() && stk.back() == in[inIdx]) {
                last = stk.back();
                stk.pop_back();
                inIdx++;
            }
            rc[last] = cur;
        }
        stk.push_back(cur);
    }

    // --- Bước 2: postorder bằng phương pháp hai stack ---
    // Sinh thứ tự root,right,left rồi đảo ngược -> left,right,root.
    vector<int> out;
    out.reserve(n);
    vector<int> s;
    s.reserve(n);
    s.push_back(pre[0]);
    while (!s.empty()) {
        int node = s.back();
        s.pop_back();
        out.push_back(node);
        if (lc[node]) s.push_back(lc[node]); // trái xuống trước -> phải ở trên
        if (rc[node]) s.push_back(rc[node]);
    }

    // in postorder (đảo ngược out)
    for (int i = n - 1; i >= 0; i--) {
        printf("%d", out[i]);
        putchar(i ? ' ' : '\n');
    }
    return 0;
}
