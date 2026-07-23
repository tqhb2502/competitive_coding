#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF = (ll)4e18;

int n, m;

// Dijkstra trên đồ thị g bắt đầu từ src, trả về mảng khoảng cách.
static vector<ll> dijkstra(int src, const vector<vector<pair<int,ll>>>& g) {
    vector<ll> d(n + 1, INF);
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    d[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        ll dd = top.first; int u = top.second;
        if (dd > d[u]) continue;
        for (const auto& e : g[u]) {
            int v = e.first; ll w = e.second;
            if (d[u] + w < d[v]) {
                d[v] = d[u] + w;
                pq.push({d[v], v});
            }
        }
    }
    return d;
}

int main() {
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<int> ea(m), eb(m);
    vector<ll>  ec(m);
    vector<vector<pair<int,ll>>> adj(n + 1), radj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b; ll c;
        scanf("%d %d %lld", &a, &b, &c);
        ea[i] = a; eb[i] = b; ec[i] = c;
        adj[a].push_back({b, c});
        radj[b].push_back({a, c});
    }

    vector<ll> d1 = dijkstra(1, adj);   // 1 -> v
    vector<ll> d2 = dijkstra(n, radj);  // v -> n
    ll D = d1[n];                       // chi phí tối ưu

    // Các đỉnh nằm trên một đường đi ngắn nhất nào đó: d1[v] + d2[v] == D.
    // Q = tập giá trị d1 phân biệt của những đỉnh này (nén tọa độ).
    vector<char> onDag(n + 1, 0);
    vector<ll> Q;
    Q.reserve(n);
    for (int v = 1; v <= n; v++) {
        if (d1[v] < INF && d2[v] < INF && d1[v] + d2[v] == D) {
            onDag[v] = 1;
            Q.push_back(d1[v]);
        }
    }
    sort(Q.begin(), Q.end());
    Q.erase(unique(Q.begin(), Q.end()), Q.end());
    int qn = (int)Q.size();

    // Số đỉnh cùng một mức khoảng cách.
    vector<int> cnt(qn, 0);
    for (int v = 1; v <= n; v++) if (onDag[v]) {
        int idx = (int)(lower_bound(Q.begin(), Q.end(), d1[v]) - Q.begin());
        cnt[idx]++;
    }

    // Sweep phủ đoạn: mỗi cạnh DAG (a->b) cho khoảng mở (d1[a], d1[b]).
    vector<ll> diff(qn + 1, 0);
    for (int i = 0; i < m; i++) {
        int a = ea[i], b = eb[i]; ll c = ec[i];
        if (d1[a] < INF && d2[b] < INF && d1[a] + c + d2[b] == D) {
            ll l = d1[a], r = d1[a] + c; // r == d1[b]
            int lo = (int)(upper_bound(Q.begin(), Q.end(), l) - Q.begin());       // tọa độ đầu tiên > l
            int hi = (int)(lower_bound(Q.begin(), Q.end(), r) - Q.begin()) - 1;   // tọa độ cuối cùng < r
            if (lo <= hi) { diff[lo] += 1; diff[hi + 1] -= 1; }
        }
    }
    vector<ll> cov(qn, 0);
    ll run = 0;
    for (int i = 0; i < qn; i++) { run += diff[i]; cov[i] = run; }

    // Đỉnh bắt buộc: duy nhất tại mức đó & không bị cạnh nào nhảy qua.
    vector<int> ans;
    for (int v = 1; v <= n; v++) if (onDag[v]) {
        int idx = (int)(lower_bound(Q.begin(), Q.end(), d1[v]) - Q.begin());
        if (cnt[idx] == 1 && cov[idx] == 0) ans.push_back(v);
    }
    sort(ans.begin(), ans.end());

    printf("%d\n", (int)ans.size());
    for (int i = 0; i < (int)ans.size(); i++)
        printf("%d%c", ans[i], (i + 1 < (int)ans.size()) ? ' ' : '\n');
    if (ans.empty()) printf("\n");
    return 0;
}
