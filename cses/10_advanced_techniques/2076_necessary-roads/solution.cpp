// Necessary Roads (CSES 2076)
// https://cses.fi/problemset/task/2076
// Tim tat ca bridge (cau) cua do thi vo huong lien thong bang thuat toan
// Tarjan low-link, DFS iterative de tranh tran stack (n <= 1e5).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    // Forward-star adjacency. Moi canh vo huong i luu thanh 2 directed edge:
    // id 2i va 2i+1, nen edge e va e^1 la hai chieu cua cung mot canh.
    vector<int> head(n + 1, -1);
    vector<int> to(2 * m), nxt(2 * m);
    vector<int> ea(m), eb(m); // hai dau mut goc cua canh i
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

    // Stack tuong minh: (dinh u, edge id cua canh cha pe)
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
                int e = cur[u];
                cur[u] = nxt[e];
                int v = to[e];
                if (e == (pe ^ 1)) continue; // bo qua chinh canh cha
                if (!disc[v]) {
                    disc[v] = low[v] = ++timer;
                    cur[v] = head[v];
                    st.push_back({v, e});
                } else {
                    if (disc[v] < low[u]) low[u] = disc[v];
                }
            } else {
                // Duyet xong u -> pop, cap nhat cho cha va kiem tra bridge.
                st.pop_back();
                if (!st.empty()) {
                    int p = st.back().first;
                    if (low[u] < low[p]) low[p] = low[u];
                    if (low[u] > disc[p]) {
                        int road = pe >> 1; // pe la canh (p, u)
                        bridges.push_back({ea[road], eb[road]});
                    }
                }
            }
        }
    }

    // Xuat ket qua bang buffer de nhanh.
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
