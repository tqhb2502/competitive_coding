#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Danh sách kề dạng forward-star. Mỗi cạnh vô hướng i lưu thành hai directed
    // edge id 2i và 2i+1, nên e và e^1 là hai chiều của cùng một cạnh.
    vector<int> head(n + 1, -1);
    vector<int> to(2 * m), nxt(2 * m);
    vector<int> ea(m), eb(m); // hai đầu mút gốc của cạnh i
    int ecnt = 0;

    auto addEdge = [&](int u, int v) {
        to[ecnt] = v;
        nxt[ecnt] = head[u];
        head[u] = ecnt;
        ecnt++;
    };

    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        ea[i] = a;
        eb[i] = b;
        addEdge(a, b);
        addEdge(b, a);
    }

    vector<int> disc(n + 1, 0), low(n + 1, 0), cur(n + 1, -1);
    int timer = 0;

    vector<pair<int, int>> bridges;

    // Stack tường minh cho DFS iterative: mỗi khung lưu (đỉnh u, edge id cạnh cha pe).
    vector<pair<int, int>> st;
    st.reserve(n);

    for (int s = 1; s <= n; s++) {
        if (disc[s]) continue;
        disc[s] = low[s] = ++timer;
        cur[s] = head[s];
        st.push_back({s, -1});

        while (!st.empty()) {
            int u = st.back().first;
            int pe = st.back().second;

            if (cur[u] != -1) {
                // Còn cạnh chưa duyệt từ u: lấy cạnh e rồi tiến con trỏ.
                int e = cur[u];
                cur[u] = nxt[e];
                int v = to[e];
                if (e == (pe ^ 1)) continue; // bỏ qua chính cạnh cha
                if (!disc[v]) {
                    // Tree edge: thăm đỉnh mới v và đẩy vào stack.
                    disc[v] = low[v] = ++timer;
                    cur[v] = head[v];
                    st.push_back({v, e});
                } else {
                    // Back edge tới đỉnh đã thăm: cập nhật low[u] bằng disc[v].
                    if (disc[v] < low[u]) low[u] = disc[v];
                }
            } else {
                // Duyệt xong u -> pop, truyền low về cha và kiểm tra điều kiện bridge.
                st.pop_back();
                if (!st.empty()) {
                    int p = st.back().first;
                    if (low[u] < low[p]) low[p] = low[u];
                    // low[u] > disc[p]: không có back edge leo qua p -> (p, u) là cầu.
                    if (low[u] > disc[p]) {
                        int road = pe >> 1; // pe chính là cạnh (p, u)
                        bridges.push_back({ea[road], eb[road]});
                    }
                }
            }
        }
    }

    // Xuất kết quả qua buffer chuỗi để tăng tốc I/O.
    string out;
    out.reserve(bridges.size() * 12 + 16);
    out += to_string((long long)bridges.size());
    out += '\n';
    char buf[24];
    for (auto &pr : bridges) {
        int len = snprintf(buf, sizeof(buf), "%d %d\n", pr.first, pr.second);
        out.append(buf, len);
    }
    fputs(out.c_str(), stdout);

    return 0;
}
