// Strongly Connected Edges — CSES 2177
// https://cses.fi/problemset/task/2177
//
// Robbins' theorem: an undirected graph has a strongly connected orientation
// iff it is connected and bridgeless (2-edge-connected).
// Iterative DFS (Tarjan) to detect bridges + connectivity, and orient edges:
//   tree edges parent -> child, back edges deeper -> ancestor.
// Complexity: O(n + m).

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<int> eu(m), ev(m);
    vector<int> deg(n + 2, 0);
    for (int e = 0; e < m; e++) {
        int a, b;
        scanf("%d %d", &a, &b);
        eu[e] = a; ev[e] = b;
        deg[a]++; deg[b]++;
    }

    // CSR adjacency (undirected): store (neighbor, edge id).
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

    vector<int> disc(n + 1, 0), low(n + 1, 0);
    // Oriented result per edge: outU[e] -> outV[e].
    vector<int> outU(m), outV(m);
    int timer = 0;
    bool hasBridge = false;

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
            st.back().ptr++;             // advance BEFORE any push_back
            int e = adjEdge[idx];
            int w = adjTo[idx];
            if (e == pe) continue;       // skip the edge we came from
            if (disc[w] == 0) {
                // tree edge: orient parent -> child
                outU[e] = u; outV[e] = w;
                disc[w] = low[w] = ++timer;
                st.push_back({w, e, head[w]});   // invalidates refs; fine, done above
            } else if (disc[w] < disc[u]) {
                // back edge going up: orient deeper -> ancestor
                if (disc[w] < low[u]) low[u] = disc[w];
                outU[e] = u; outV[e] = w;
            }
            // else disc[w] > disc[u]: already oriented from w's (deeper) side
        } else {
            int lu = low[u];
            st.pop_back();
            if (!st.empty()) {
                int p = st.back().u;
                if (lu < low[p]) low[p] = lu;
                if (lu > disc[p]) hasBridge = true;   // tree edge (p,u) is a bridge
            }
        }
    }

    // Connectivity: every node must have been visited.
    bool connected = true;
    for (int i = 1; i <= n && connected; i++)
        if (disc[i] == 0) connected = false;

    if (!connected || hasBridge) {
        printf("IMPOSSIBLE\n");
        return 0;
    }

    // Output oriented edges (input order; any order is accepted by the judge).
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
