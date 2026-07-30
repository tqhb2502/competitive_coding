#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<int> ea(m), eb(m);
    vector<vector<int>> adj(n + 1), radj(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        ea[i] = a; eb[i] = b;
        adj[a].push_back(b);
        radj[b].push_back(a);
    }

    // ---- Kosaraju pass 1: finish order trên đồ thị gốc (DFS lặp) ----
    vector<int> order;
    order.reserve(n);
    vector<char> vis(n + 1, 0);
    vector<int> it(n + 1, 0);
    vector<int> st;
    st.reserve(n);
    for (int s = 1; s <= n; s++) {
        if (vis[s]) continue;
        st.clear();
        st.push_back(s);
        vis[s] = 1;
        while (!st.empty()) {
            int u = st.back();
            if (it[u] < (int)adj[u].size()) {
                int v = adj[u][it[u]++];
                if (!vis[v]) { vis[v] = 1; st.push_back(v); }
            } else {
                order.push_back(u);
                st.pop_back();
            }
        }
    }

    // ---- Kosaraju pass 2: DFS trên đồ thị ngược theo thứ tự finish giảm ----
    vector<int> comp(n + 1, -1);
    int c = 0;
    vector<int> it2(n + 1, 0);
    for (int i = n - 1; i >= 0; i--) {
        int s = order[i];
        if (comp[s] != -1) continue;
        st.clear();
        st.push_back(s);
        comp[s] = c;
        while (!st.empty()) {
            int u = st.back();
            if (it2[u] < (int)radj[u].size()) {
                int v = radj[u][it2[u]++];
                if (comp[v] == -1) { comp[v] = c; st.push_back(v); }
            } else {
                st.pop_back();
            }
        }
        c++;
    }

    // Chỉ 1 SCC -> đã strongly connected.
    if (c == 1) {
        printf("0\n");
        return 0;
    }

    // ---- Condensation ----
    vector<char> hasIn(c, 0), hasOut(c, 0);
    vector<vector<int>> cadj(c);
    vector<int> rep(c);
    for (int v = 1; v <= n; v++) rep[comp[v]] = v; // đại diện bất kỳ trong SCC
    for (int i = 0; i < m; i++) {
        int cu = comp[ea[i]], cv = comp[eb[i]];
        if (cu != cv) {
            cadj[cu].push_back(cv);
            hasOut[cu] = 1;
            hasIn[cv] = 1;
        }
    }

    vector<int> sources, sinks;
    for (int i = 0; i < c; i++) {
        if (!hasIn[i]) sources.push_back(i);
        if (!hasOut[i]) sinks.push_back(i);
    }

    // ---- Ghép source -> sink bằng greedy DFS (visited toàn cục, DỪNG NGAY khi gặp
    //      sink tự do đầu tiên - "early return"). Cách này bảo đảm P1/P2 và O(c + cạnh)
    //      vì mỗi đỉnh chỉ được vào đúng một lần. ----
    vector<char> isSink(c, 0), usedSink(c, 0), vis2(c, 0);
    for (int t : sinks) isSink[t] = 1;
    vector<int> cit(c, 0);
    vector<int> matchedV, matchedW, extraSources;

    for (int src : sources) {
        if (vis2[src]) continue; // source (in-deg 0) không bao giờ bị thăm trước
        int freeSink = -1;
        vis2[src] = 1;
        if (isSink[src]) {
            // source đồng thời là sink => SCC cô lập, ghép với chính nó
            freeSink = src;
        } else {
            st.clear();
            st.push_back(src);
            while (!st.empty()) {
                int u = st.back();
                if (cit[u] < (int)cadj[u].size()) {
                    int v = cadj[u][cit[u]++];
                    if (!vis2[v]) {
                        vis2[v] = 1;
                        if (isSink[v]) {
                            if (!usedSink[v]) { freeSink = v; break; } // dừng ngay
                            // sink đã dùng: không có cạnh ra, bỏ qua
                        } else {
                            st.push_back(v);
                        }
                    }
                } else {
                    st.pop_back();
                }
            }
        }
        if (freeSink != -1) {
            usedSink[freeSink] = 1;
            matchedV.push_back(src);
            matchedW.push_back(freeSink);
        } else {
            extraSources.push_back(src);
        }
    }

    vector<int> extraSinks;
    for (int t : sinks) if (!usedSink[t]) extraSinks.push_back(t);

    int p = (int)matchedV.size(); // p >= 1 vì c >= 2
    vector<pair<int,int>> res; // cặp SCC (u -> v)

    // 1) Chu trình các cặp matched: w_i -> v_{(i+1) mod p}
    for (int i = 0; i < p; i++) {
        res.push_back({matchedW[i], matchedV[(i + 1) % p]});
    }

    int x = (int)extraSources.size();
    int y = (int)extraSinks.size();
    int k = min(x, y);
    // 2) Ghép extra sink với extra source
    for (int j = 0; j < k; j++) {
        res.push_back({extraSinks[j], extraSources[j]});
    }
    // 3) Phần dư nối vào Big
    if (x > y) {
        for (int j = k; j < x; j++) res.push_back({matchedW[0], extraSources[j]});
    } else if (y > x) {
        for (int j = k; j < y; j++) res.push_back({extraSinks[j], matchedV[0]});
    }

    // ---- Xuất ----
    // res.size() == max(#source, #sink)
    string out;
    out.reserve(res.size() * 12 + 16);
    out += to_string((long long)res.size());
    out += '\n';
    for (auto &pr : res) {
        out += to_string(rep[pr.first]);
        out += ' ';
        out += to_string(rep[pr.second]);
        out += '\n';
    }
    fputs(out.c_str(), stdout);
    return 0;
}
