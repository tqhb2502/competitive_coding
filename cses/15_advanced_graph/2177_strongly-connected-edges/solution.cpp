#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    // Đọc và lưu danh sách cạnh vô hướng, đồng thời đếm bậc mỗi đỉnh.
    vector<int> eu(m), ev(m);
    vector<int> deg(n + 2, 0);
    for (int e = 0; e < m; e++) {
        int a, b;
        scanf("%d %d", &a, &b);
        eu[e] = a; ev[e] = b;
        deg[a]++; deg[b]++;
    }

    // Dựng danh sách kề dạng CSR (vô hướng): mỗi cung lưu (đỉnh kề, id cạnh).
    vector<int> head(n + 2, 0);
    for (int i = 1; i <= n; i++) head[i + 1] = head[i] + deg[i];
    int totalArc = head[n + 1];
    vector<int> adjTo(totalArc), adjEdge(totalArc);
    vector<int> pos(n + 2);
    for (int i = 1; i <= n + 1; i++) pos[i] = head[i];
    for (int e = 0; e < m; e++) {
        int a = eu[e], b = ev[e];
        adjTo[pos[a]] = b; adjEdge[pos[a]] = e; pos[a]++;
        adjTo[pos[b]] = a; adjEdge[pos[b]] = e; pos[b]++;
    }

    // disc[]: thời điểm thăm; low[]: đỉnh nhỏ nhất tới được (Tarjan phát hiện cầu).
    vector<int> disc(n + 1, 0), low(n + 1, 0);
    // Kết quả định hướng của mỗi cạnh: outU[e] -> outV[e].
    vector<int> outU(m), outV(m);
    int timer = 0;
    bool hasBridge = false;

    // DFS lặp bằng stack tường minh (tránh tràn stack với n, m lớn).
    // Mỗi frame lưu: đỉnh u, id cạnh cha pe, con trỏ quét danh sách kề ptr.
    struct Frame { int u; int pe; int ptr; };
    vector<Frame> st;
    st.reserve(n + 5);

    disc[1] = low[1] = ++timer;
    st.push_back({1, -1, head[1]});

    while (!st.empty()) {
        int u = st.back().u;
        int pe = st.back().pe;
        if (st.back().ptr < head[u + 1]) {
            int idx = st.back().ptr;
            st.back().ptr++;             // tăng con trỏ TRƯỚC mọi push_back
            int e = adjEdge[idx];
            int w = adjTo[idx];
            if (e == pe) continue;       // bỏ qua đúng cạnh vừa đi tới (cạnh cha)
            if (disc[w] == 0) {
                // Cạnh cây: định hướng cha -> con
                outU[e] = u; outV[e] = w;
                disc[w] = low[w] = ++timer;
                st.push_back({w, e, head[w]});   // đã tăng ptr ở trên nên an toàn
            } else if (disc[w] < disc[u]) {
                // Back edge đi lên: định hướng đỉnh sâu hơn -> tổ tiên
                if (disc[w] < low[u]) low[u] = disc[w];
                outU[e] = u; outV[e] = w;
            }
            // else disc[w] > disc[u]: đã được định hướng từ phía w (sâu hơn)
        } else {
            // Quét xong đỉnh u: cập nhật low của cha và kiểm tra cầu.
            int lu = low[u];
            st.pop_back();
            if (!st.empty()) {
                int p = st.back().u;
                if (lu < low[p]) low[p] = lu;
                if (lu > disc[p]) hasBridge = true;   // cạnh cây (p,u) là cầu
            }
        }
    }

    // Kiểm tra liên thông: mọi đỉnh đều phải được thăm.
    bool connected = true;
    for (int i = 1; i <= n && connected; i++)
        if (disc[i] == 0) connected = false;

    // Theo định lý Robbins: có cầu hoặc không liên thông => vô nghiệm.
    if (!connected || hasBridge) {
        printf("IMPOSSIBLE\n");
        return 0;
    }

    // In các cạnh đã định hướng theo thứ tự đầu vào (special judge chấp nhận mọi thứ tự).
    string out;
    out.reserve((size_t)m * 12);
    char buf[32];
    for (int e = 0; e < m; e++) {
        int len = snprintf(buf, sizeof(buf), "%d %d\n", outU[e], outV[e]);
        out.append(buf, len);
    }
    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
