#include <bits/stdc++.h>
using namespace std;

// DSU-with-rollback: union by size, KHÔNG path compression để có thể undo rẻ tiền.
struct RollbackDSU {
    vector<int> par, sz;
    int comp;                       // số thành phần liên thông hiện tại
    vector<pair<int,int>> ops;      // các cặp (root, child) của union thật sự, dùng để rollback

    void init(int n) {
        par.resize(n + 1);
        sz.assign(n + 1, 1);
        iota(par.begin(), par.end(), 0);
        comp = n;
        ops.clear();
    }
    // Không path compression -> giữ được cấu trúc cây để rollback
    int find(int x) {
        while (par[x] != x) x = par[x];
        return x;
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;         // không thay đổi cấu trúc
        if (sz[a] < sz[b]) swap(a, b);
        par[b] = a;
        sz[a] += sz[b];
        --comp;                     // hai thành phần gộp lại -> giảm một
        ops.emplace_back(a, b);     // lưu lại để hoàn tác
    }
    // Hoàn tác các union cho tới khi stack ops về đúng checkpoint
    void rollback(size_t checkpoint) {
        while (ops.size() > checkpoint) {
            auto [a, b] = ops.back();
            ops.pop_back();
            sz[a] -= sz[b];
            par[b] = b;
            ++comp;
        }
    }
};

int K;                                          // số sự kiện (các trạng thái là 0..K)
vector<vector<pair<int,int>>> seg;              // các cạnh lưu tại mỗi node segment tree
RollbackDSU dsu;
vector<int> ans;

// Thêm cạnh e vào O(log K) node phủ khoảng [l, r] (node hiện tại phủ [nl, nr])
void addEdge(int node, int nl, int nr, int l, int r, const pair<int,int>& e) {
    if (r < nl || nr < l) return;
    if (l <= nl && nr <= r) { seg[node].push_back(e); return; }
    int mid = (nl + nr) / 2;
    addEdge(2 * node,     nl,      mid, l, r, e);
    addEdge(2 * node + 1, mid + 1, nr,  l, r, e);
}

// DFS segment tree: union các cạnh tại node, xuống con, rồi rollback khi trả về
void dfs(int node, int nl, int nr) {
    size_t cp = dsu.ops.size();                 // ghi lại checkpoint trước khi union
    for (const auto& e : seg[node]) dsu.unite(e.first, e.second);
    if (nl == nr) {
        // Node lá: comp hiện tại là số thành phần liên thông đúng của trạng thái nl
        ans[nl] = dsu.comp;
    } else {
        int mid = (nl + nr) / 2;
        dfs(2 * node,     nl,      mid);
        dfs(2 * node + 1, mid + 1, nr);
    }
    dsu.rollback(cp);                           // hoàn tác các union của node này
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m >> K)) return 0;

    // Mã hóa cặp đỉnh thành một số: min*BASE + max (n <= 1e5 nên BASE = 100001 an toàn)
    const long long BASE = 100001LL;
    auto enc = [&](int a, int b) -> long long {
        if (a > b) swap(a, b);
        return (long long)a * BASE + b;
    };

    // active[key] = stack các trạng thái start của các bản sao cạnh đang "mở"
    unordered_map<long long, vector<int>> active;
    active.reserve(2 * (m + K) + 16);

    // Các cạnh ban đầu tồn tại từ trạng thái 0
    for (int i = 0; i < m; ++i) {
        int a, b; cin >> a >> b;
        active[enc(a, b)].push_back(0);
    }

    // Segment tree trên các trạng thái [0, K] -> K+1 lá
    seg.assign(4 * (K + 1), {});
    ans.assign(K + 1, 0);

    // Xử lý từng sự kiện, sinh khoảng tuổi thọ cho cạnh bị xóa
    for (int i = 1; i <= K; ++i) {
        int t, a, b; cin >> t >> a >> b;
        long long key = enc(a, b);
        if (t == 1) {
            active[key].push_back(i);           // cạnh xuất hiện từ trạng thái i
        } else {
            // Xóa: cạnh tồn tại trong khoảng [start, i-1]
            auto& st = active[key];
            int start = st.back();
            st.pop_back();
            addEdge(1, 0, K, start, i - 1, make_pair(a, b));
        }
    }

    // Các cạnh không bao giờ bị xóa tồn tại đến tận trạng thái K
    for (auto& kv : active) {
        if (kv.second.empty()) continue;
        int a = (int)(kv.first / BASE);
        int b = (int)(kv.first % BASE);
        for (int start : kv.second) {
            addEdge(1, 0, K, start, K, make_pair(a, b));
        }
    }

    dsu.init(n);
    dfs(1, 0, K);

    // In ra K+1 số thành phần liên thông
    string out;
    out.reserve((size_t)(K + 1) * 7);
    for (int i = 0; i <= K; ++i) {
        out += to_string(ans[i]);
        out += (i == K ? '\n' : ' ');
    }
    fputs(out.c_str(), stdout);
    return 0;
}
