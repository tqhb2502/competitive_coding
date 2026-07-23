#include <bits/stdc++.h>
using namespace std;

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    while (t--) {
        int n;
        scanf("%d", &n);

        // Đọc n-1 cạnh cho mỗi cây, dựng danh sách kề vô hướng.
        vector<vector<int>> adj1(n + 1), adj2(n + 1);
        for (int i = 0; i < n - 1; i++) {
            int a, b; scanf("%d %d", &a, &b);
            adj1[a].push_back(b);
            adj1[b].push_back(a);
        }
        for (int i = 0; i < n - 1; i++) {
            int a, b; scanf("%d %d", &a, &b);
            adj2[a].push_back(b);
            adj2[b].push_back(a);
        }

        // Map chính tắc dùng chung cho cả hai cây trong cùng test.
        map<vector<int>, int> canon;
        int idCnt = 0;

        // Trả về id chính tắc của gốc (đỉnh 1) của cây 'adj'.
        auto rootId = [&](vector<vector<int>>& adj) -> int {
            vector<int> parent(n + 1, 0), order;
            vector<char> vis(n + 1, 0);
            order.reserve(n);

            // BFS lặp từ gốc 1.
            vector<int> q;
            q.reserve(n);
            q.push_back(1);
            vis[1] = 1;
            parent[1] = -1;
            for (size_t head = 0; head < q.size(); head++) {
                int u = q[head];
                order.push_back(u);
                for (int v : adj[u]) {
                    if (!vis[v]) {
                        vis[v] = 1;
                        parent[v] = u;
                        q.push_back(v);
                    }
                }
            }

            vector<int> id(n + 1, 0);
            // Xử lý ngược thứ tự BFS => con trước cha (post-order).
            for (int i = (int)order.size() - 1; i >= 0; i--) {
                int u = order[i];
                // Gom id của các con rồi SẮP XẾP để khoá không phụ thuộc thứ tự (multiset).
                vector<int> ch;
                for (int v : adj[u])
                    if (v != parent[u]) ch.push_back(id[v]);
                sort(ch.begin(), ch.end());
                // Tra khoá trong map chung: có sẵn thì tái dùng id, chưa có thì cấp id mới.
                auto it = canon.find(ch);
                if (it == canon.end()) {
                    id[u] = idCnt;
                    canon.emplace(move(ch), idCnt);
                    idCnt++;
                } else {
                    id[u] = it->second;
                }
            }
            return id[1];
        };

        // Hai cây đẳng cấu (có gốc) <=> id chính tắc của gốc (đỉnh 1) trùng nhau.
        int r1 = rootId(adj1);
        int r2 = rootId(adj2);
        printf(r1 == r2 ? "YES\n" : "NO\n");
    }
    return 0;
}
