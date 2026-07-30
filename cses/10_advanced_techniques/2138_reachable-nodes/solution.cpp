#include <bits/stdc++.h>
using namespace std;

const int MAXN = 50000;

int n, m;
vector<int> adj[MAXN + 1];
int indeg[MAXN + 1];

// Mỗi đỉnh giữ một bitset (bit i-1 ứng với đỉnh i). ~313 MB khi n = 50000.
vector<bitset<MAXN>> reach;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // Đọc đồ thị và đếm bậc vào của từng đỉnh.
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        adj[a].push_back(b);
        indeg[b]++;
    }

    // Thuật toán Kahn (lặp, không đệ quy) -> lấy một thứ tự topo, các đỉnh nguồn trước.
    vector<int> topo;
    topo.reserve(n);
    // Bản sao bậc vào để có thể giảm dần mà không phá dữ liệu gốc.
    vector<int> deg(indeg + 1, indeg + n + 1); // deg[i] = bậc vào của đỉnh (i+1)
    queue<int> q;
    for (int i = 1; i <= n; ++i)
        if (deg[i - 1] == 0) q.push(i);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topo.push_back(u);
        for (int v : adj[u]) {
            if (--deg[v - 1] == 0) q.push(v);
        }
    }

    reach.assign(n + 1, bitset<MAXN>());

    // Duyệt ngược thứ tự topo: mọi đỉnh kề sau đã được tính xong trước u.
    // reach[u] = {u} hợp (hợp reach[v] với mọi cạnh u -> v).
    for (int idx = (int)topo.size() - 1; idx >= 0; --idx) {
        int u = topo[idx];
        reach[u].set(u - 1);
        for (int v : adj[u]) {
            reach[u] |= reach[v];
        }
    }

    // Đáp án mỗi đỉnh là số bit đang bật (popcount), in ra cách nhau bởi dấu cách.
    string out;
    out.reserve(n * 7);
    for (int i = 1; i <= n; ++i) {
        out += to_string((long long)reach[i].count());
        out += (i == n ? '\n' : ' ');
    }
    cout << out;
    return 0;
}
