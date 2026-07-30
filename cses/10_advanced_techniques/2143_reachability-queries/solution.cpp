#include <bits/stdc++.h>
using namespace std;

// Đọc số nguyên nhanh trực tiếp từ bộ đệm nhị phân (fast input).
namespace fio {
    const int BS = 1 << 16;
    char buf[BS];
    int len = 0, pos = 0;
    inline int gc() {
        if (pos == len) { len = (int)fread(buf, 1, BS, stdin); pos = 0; if (len <= 0) return -1; }
        return buf[pos++];
    }
    inline int readInt() {
        int c = gc();
        while (c != -1 && (c < '0' || c > '9')) c = gc();
        int x = 0;
        while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
        return x;
    }
}

int main() {
    int n = fio::readInt();
    int m = fio::readInt();
    int q = fio::readInt();

    // Lưu danh sách cạnh và đếm bậc ra của từng đỉnh.
    vector<int> ea(m), eb(m);
    vector<int> deg(n + 1, 0);
    for (int i = 0; i < m; i++) {
        int a = fio::readInt() - 1, b = fio::readInt() - 1;
        ea[i] = a; eb[i] = b; deg[a]++;
    }

    // Xây danh sách kề ra (out-adjacency) dạng CSR để duyệt nhanh.
    vector<int> head(n + 1, 0);
    for (int i = 0; i < n; i++) head[i + 1] = head[i] + deg[i];
    vector<int> adj(m);
    {
        vector<int> cur(head.begin(), head.end());
        for (int i = 0; i < m; i++) adj[cur[ea[i]]++] = eb[i];
    }

    // Tìm SCC bằng Tarjan dạng lặp (iterative) để tránh tràn stack khi đệ quy sâu.
    // Tarjan đánh số component theo thứ tự ngược tô-pô (reverse-topological).
    vector<int> disc(n, -1), low(n, 0), comp(n, -1), it(n);
    for (int i = 0; i < n; i++) it[i] = head[i];
    vector<char> onstk(n, 0);
    vector<int> S;  S.reserve(n);   // stack các đỉnh đang xét của Tarjan
    vector<int> cs; cs.reserve(n);  // call-stack tường minh thay cho đệ quy DFS
    int timer = 0, C = 0;
    for (int s = 0; s < n; s++) {
        if (disc[s] != -1) continue;
        cs.push_back(s);
        disc[s] = low[s] = timer++; onstk[s] = 1; S.push_back(s);
        while (!cs.empty()) {
            int u = cs.back();
            if (it[u] < head[u + 1]) {
                // Còn cạnh chưa duyệt của u: đi tới đỉnh kề v.
                int v = adj[it[u]++];
                if (disc[v] == -1) {
                    disc[v] = low[v] = timer++; onstk[v] = 1; S.push_back(v);
                    cs.push_back(v);
                } else if (onstk[v]) {
                    if (disc[v] < low[u]) low[u] = disc[v];
                }
            } else {
                // Duyệt xong u: nếu u là gốc SCC thì bóc toàn bộ component ra khỏi stack.
                if (low[u] == disc[u]) {
                    while (true) {
                        int w = S.back(); S.pop_back();
                        onstk[w] = 0; comp[w] = C;
                        if (w == u) break;
                    }
                    C++;
                }
                cs.pop_back();
                if (!cs.empty()) {
                    // Lan truyền low về cha trong DFS.
                    int p = cs.back();
                    if (low[u] < low[p]) low[p] = low[u];
                }
            }
        }
    }

    // Nén thành DAG condensation: cạnh chéo comp(u)->comp(v) chỉ tồn tại khi
    // comp(v) < comp(u) (do Tarjan đánh số theo thứ tự ngược tô-pô).
    // Đếm bậc ra của mỗi component rồi dựng CSR trên DAG.
    vector<int> cdeg(C, 0);
    for (int i = 0; i < m; i++) {
        int cu = comp[ea[i]], cv = comp[eb[i]];
        if (cu != cv) cdeg[cu]++;
    }
    vector<int> chead(C + 1, 0);
    for (int i = 0; i < C; i++) chead[i + 1] = chead[i] + cdeg[i];
    vector<int> cadj(chead[C]);
    {
        vector<int> cur(chead.begin(), chead.end());
        for (int i = 0; i < m; i++) {
            int cu = comp[ea[i]], cv = comp[eb[i]];
            if (cu != cv) cadj[cur[cu]++] = cv;
        }
    }

    // Tính transitive closure bằng bitset: mỗi component dùng W = ceil(C/64) từ 64-bit.
    // Xử lý id tăng dần nên mọi successor (id nhỏ hơn) đã tính xong trước:
    //   reach[c] = {c} hợp union các reach[d] với mọi cạnh DAG c -> d.
    int W = (C + 63) / 64;
    uint64_t* reach = new uint64_t[(size_t)C * W]();
    for (int c = 0; c < C; c++) {
        uint64_t* rc = reach + (size_t)c * W;
        rc[c >> 6] |= (1ULL << (c & 63));   // đường chéo: c luôn tới được chính nó
        for (int e = chead[c]; e < chead[c + 1]; e++) {
            // Gộp bao đóng của successor bằng phép OR trên từng từ 64-bit.
            const uint64_t* rd = reach + (size_t)cadj[e] * W;
            for (int j = 0; j < W; j++) rc[j] |= rd[j];
        }
    }

    // Trả lời truy vấn: (a,b) đạt được khi bit comp(b) bật trong reach[comp(a)].
    string out;
    out.reserve((size_t)q * 4);
    for (int i = 0; i < q; i++) {
        int a = fio::readInt() - 1, b = fio::readInt() - 1;
        int ca = comp[a], cb = comp[b];
        const uint64_t* rc = reach + (size_t)ca * W;
        if ((rc[cb >> 6] >> (cb & 63)) & 1ULL) out += "YES\n";
        else out += "NO\n";
    }
    fwrite(out.data(), 1, out.size(), stdout);

    delete[] reach;
    return 0;
}
