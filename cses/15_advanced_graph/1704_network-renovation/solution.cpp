// Network Renovation - CSES 1704
// https://cses.fi/problemset/task/1704
//
// Tree augmentation to 2-edge-connectivity (Eswaran-Tarjan).
// Số cạnh mới tối thiểu = ceil(L/2) với L = số lá (đỉnh bậc 1).
// Chọn root là một đỉnh không phải lá, DFS lấy danh sách lá theo thứ tự duyệt,
// rồi nối leaves[i] với leaves[i + L/2]; nếu L lẻ nối thêm (leaves[L-1], leaves[0]).

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    vector<vector<int>> adj(n + 1);
    vector<int> deg(n + 1, 0);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        adj[a].push_back(b);
        adj[b].push_back(a);
        deg[a]++;
        deg[b]++;
    }

    // Chọn root là một đỉnh KHÔNG phải lá (bậc >= 2). Với n >= 3 luôn tồn tại.
    int root = 1;
    for (int v = 1; v <= n; v++) {
        if (deg[v] >= 2) { root = v; break; }
    }

    // DFS lặp (iterative) theo pre-order, ghi lại các lá theo thứ tự duyệt.
    // Trong cây, mỗi đỉnh chỉ được đẩy vào stack một lần (chỉ cha đẩy con),
    // nên lá của mỗi subtree là một đoạn liên tiếp trong danh sách.
    vector<int> leaves;
    vector<char> vis(n + 1, 0);
    vector<int> st;
    st.reserve(n);
    st.push_back(root);
    while (!st.empty()) {
        int u = st.back();
        st.pop_back();
        if (vis[u]) continue;
        vis[u] = 1;
        if (deg[u] == 1) leaves.push_back(u);   // root có bậc >= 2 nên không bị tính là lá
        for (int w : adj[u]) if (!vis[w]) st.push_back(w);
    }

    int cnt = (int)leaves.size();
    int m = cnt / 2;
    int k = (cnt + 1) / 2;

    printf("%d\n", k);
    for (int i = 0; i < m; i++) {
        printf("%d %d\n", leaves[i], leaves[i + m]);
    }
    if (cnt & 1) {
        printf("%d %d\n", leaves[cnt - 1], leaves[0]);
    }

    return 0;
}
