// Even Outdegree Edges — CSES 2179
// https://cses.fi/problemset/task/2179
//
// Định hướng các cạnh vô hướng để mọi đỉnh có bậc ra (outdegree) chẵn.
// Cách làm: dựng cây DFS (spanning forest) bằng DFS LẶP, định hướng back edge
// tùy ý khi phát hiện, rồi khi mỗi đỉnh (khác gốc) kết thúc thì chọn hướng cho
// cạnh cây nối cha để ép bậc ra của đỉnh đó thành chẵn (post-order fix parity).
// Cuối cùng nếu còn đỉnh bậc ra lẻ (chỉ có thể là gốc của thành phần có số cạnh
// lẻ) thì in "IMPOSSIBLE". Độ phức tạp O(n + m).

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<int> eu(m), ev(m);
    vector<int> deg(n + 2, 0);
    for (int i = 0; i < m; i++) {
        scanf("%d %d", &eu[i], &ev[i]);
        deg[eu[i]]++;
        deg[ev[i]]++;
    }

    // CSR adjacency: neighbor + edge index.
    vector<int> start(n + 2, 0);
    for (int v = 1; v <= n; v++) start[v + 1] = start[v] + deg[v];
    vector<int> adjTo(2 * m), adjEi(2 * m);
    vector<int> pos(n + 2);
    for (int v = 1; v <= n + 1; v++) pos[v] = start[v];
    for (int i = 0; i < m; i++) {
        int a = eu[i], b = ev[i];
        adjTo[pos[a]] = b; adjEi[pos[a]] = i; pos[a]++;
        adjTo[pos[b]] = a; adjEi[pos[b]] = i; pos[b]++;
    }

    vector<char> visited(n + 1, 0), usedEdge(m, 0);
    vector<int> parent(n + 1, -1), parentEdge(n + 1, -1);
    vector<int> ptr(n + 2);
    for (int v = 1; v <= n; v++) ptr[v] = start[v];
    vector<long long> outdeg(n + 1, 0);

    // Kết quả: mỗi cạnh i được định hướng fromArr[i] -> toArr[i].
    vector<int> fromArr(m, 0), toArr(m, 0);

    vector<int> stk;
    stk.reserve(n + 1);

    for (int s = 1; s <= n; s++) {
        if (visited[s]) continue;
        visited[s] = 1;
        parent[s] = -1;
        parentEdge[s] = -1;
        stk.push_back(s);
        while (!stk.empty()) {
            int v = stk.back();
            if (ptr[v] < start[v + 1]) {
                int idx = ptr[v]++;
                int ei = adjEi[idx];
                if (usedEdge[ei]) continue;   // đã xử lý (kể cả cạnh tới cha)
                usedEdge[ei] = 1;
                int to = adjTo[idx];
                if (!visited[to]) {
                    // cạnh cây: hướng quyết định khi 'to' kết thúc
                    visited[to] = 1;
                    parent[to] = v;
                    parentEdge[to] = ei;
                    stk.push_back(to);
                } else {
                    // back edge: định hướng tùy ý v -> to
                    fromArr[ei] = v;
                    toArr[ei] = to;
                    outdeg[v]++;
                }
            } else {
                // đỉnh v kết thúc: chỉnh parity qua cạnh cây tới cha
                stk.pop_back();
                int pe = parentEdge[v];
                if (pe != -1) {
                    int p = parent[v];
                    if (outdeg[v] & 1LL) {
                        fromArr[pe] = v; toArr[pe] = p;   // v -> p
                        outdeg[v]++;
                    } else {
                        fromArr[pe] = p; toArr[pe] = v;   // p -> v
                        outdeg[p]++;
                    }
                }
            }
        }
    }

    // Kiểm tra: đỉnh bậc ra lẻ chỉ có thể là gốc của thành phần có số cạnh lẻ.
    for (int v = 1; v <= n; v++) {
        if (outdeg[v] & 1LL) {
            printf("IMPOSSIBLE\n");
            return 0;
        }
    }

    // Xuất kết quả.
    string out;
    out.reserve((size_t)m * 12 + 16);
    char buf[32];
    for (int i = 0; i < m; i++) {
        int len = sprintf(buf, "%d %d\n", fromArr[i], toArr[i]);
        out.append(buf, len);
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
