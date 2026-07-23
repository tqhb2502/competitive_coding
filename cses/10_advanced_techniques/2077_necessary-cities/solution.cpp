#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    // Danh sách kề của đồ thị vô hướng
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    // disc: thời điểm thăm; low: disc nhỏ nhất vươn tới được qua tối đa một back edge
    // parent: cha trong cây DFS; it: con trỏ cạnh kề kế tiếp; childCount: số con của gốc
    vector<int> disc(n + 1, 0), low(n + 1, 0), parent(n + 1, -1);
    vector<int> it(n + 1, 0), childCount(n + 1, 0);
    vector<char> isAP(n + 1, 0);
    int timer = 0;

    // Stack tường minh cho iterative DFS, tránh tràn stack khi n tới 1e5
    vector<int> st;
    st.reserve(n);

    for (int s = 1; s <= n; s++) {
        if (disc[s]) continue;
        parent[s] = -1;
        disc[s] = low[s] = ++timer;
        st.push_back(s);

        while (!st.empty()) {
            int u = st.back();
            if (it[u] < (int)adj[u].size()) {
                int v = adj[u][it[u]++];
                if (!disc[v]) {
                    // Cạnh cây DFS: đi xuống con v mới
                    parent[v] = u;
                    childCount[u]++;
                    disc[v] = low[v] = ++timer;
                    st.push_back(v);
                } else if (v != parent[u]) {
                    // Back edge: cập nhật low bằng disc của tổ tiên
                    low[u] = min(low[u], disc[v]);
                }
            } else {
                // Đã duyệt xong u -> pop và truyền low ngược lên cha
                st.pop_back();
                int p = parent[u];
                if (p != -1) {
                    low[p] = min(low[p], low[u]);
                    // Cha không phải gốc và cây con u không trèo lên trên p -> p là đỉnh cắt
                    if (parent[p] != -1 && low[u] >= disc[p]) {
                        isAP[p] = 1;
                    }
                }
            }
        }

        // Trường hợp gốc: có từ 2 con trở lên trong cây DFS thì gốc là đỉnh cắt
        if (childCount[s] >= 2) isAP[s] = 1;
    }

    // Thu thập các articulation point theo thứ tự tăng dần
    vector<int> res;
    for (int i = 1; i <= n; i++) {
        if (isAP[i]) res.push_back(i);
    }

    cout << res.size() << "\n";
    for (size_t i = 0; i < res.size(); i++) {
        cout << res[i];
        if (i + 1 < res.size()) cout << ' ';
    }
    cout << "\n";

    return 0;
}
