#pragma once
#include <bits/stdc++.h>
using namespace std;
// 2-SAT — gán TRUE/FALSE cho n biến thỏa mọi mệnh đề (a OR b), qua đồ thị suy diễn + SCC, O(n + m).
// Khi dùng: ràng buộc dạng "chọn 1 trong 2 / nếu ... thì ..."; mỗi mệnh đề đúng 2 literal.
// ĐPT: O(n + biên); bộ nhớ O(n + biên). Biến 0-based; literal 2*i = (x_i đúng), 2*i+1 = (x_i sai).
// Dùng: TwoSat s(n); s.addClause(i, true, j, false); s.addFact(k, true); if (s.solve()) s.value[i];
// Bẫy: solve() trả false nếu mâu thuẫn; addClause(i,fi,j,fj) nghĩa (x_i=fi HOẶC x_j=fj); dựng trước, solve sau.
// CSES: 1684 3312
struct TwoSat {
    int n;
    vector<vector<int>> adj;
    vector<int> comp;
    vector<char> value;
    // Ý nghĩa: khởi tạo bộ giải 2-SAT với n biến; dựng đồ thị suy diễn 2*n đỉnh (literal).
    // Tham số: n = số biến boolean (0-based).
    explicit TwoSat(int n) : n(n), adj(2 * n) {}
    // Ý nghĩa: mã hóa literal của mệnh đề (x_i = f) thành chỉ số đỉnh trong đồ thị.
    // Tham số: i = chỉ số biến (0-based); f = giá trị mong muốn (true/false).
    // Trả về: 2*i nếu f=true, 2*i+1 nếu f=false.
    int lit(int i, bool f) { return 2 * i + (f ? 0 : 1); }        // literal của (x_i = f)
    // Ý nghĩa: thêm cạnh suy diễn a -> b (nếu a đúng thì b đúng) vào đồ thị.
    // Tham số: a, b = chỉ số hai literal (đỉnh).
    void addImpl(int a, int b) { adj[a].push_back(b); }           // a -> b
    // Ý nghĩa: thêm mệnh đề (x_i=fi HOẶC x_j=fj) bằng hai cạnh suy diễn ¬a->b và ¬b->a.
    // Tham số: i, j = chỉ số hai biến; fi, fj = giá trị mong muốn tương ứng.
    void addClause(int i, bool fi, int j, bool fj) {              // (x_i=fi) OR (x_j=fj)
        int a = lit(i, fi), b = lit(j, fj);
        addImpl(a ^ 1, b);   // ¬a -> b
        addImpl(b ^ 1, a);   // ¬b -> a
    }
    // Ý nghĩa: buộc x_i = f cứng bằng cạnh ¬a->a (mọi nghiệm đều phải thỏa literal a).
    // Tham số: i = chỉ số biến; f = giá trị bắt buộc.
    void addFact(int i, bool f) { int a = lit(i, f); addImpl(a ^ 1, a); }  // buộc x_i = f
    // Ý nghĩa: tìm SCC (Tarjan lặp) trên đồ thị suy diễn rồi gán giá trị các biến vào value.
    // Trả về: true nếu có nghiệm (điền value[0..n-1]); false nếu mâu thuẫn (x_i và ¬x_i cùng SCC).
    bool solve() {
        int N = 2 * n, timer = 0, sc = 0;
        vector<int> num(N, -1), low(N, -1), stk, callStk, it(N, 0);
        vector<char> onStk(N, 0);
        comp.assign(N, -1);
        for (int s = 0; s < N; s++) {
            if (num[s] != -1) continue;
            callStk.push_back(s);
            while (!callStk.empty()) {
                int u = callStk.back();
                if (it[u] == 0) { num[u] = low[u] = timer++; stk.push_back(u); onStk[u] = 1; }
                if (it[u] < (int)adj[u].size()) {
                    int v = adj[u][it[u]++];
                    if (num[v] == -1) callStk.push_back(v);
                    else if (onStk[v]) low[u] = min(low[u], num[v]);
                } else {
                    if (low[u] == num[u]) {
                        while (true) {
                            int w = stk.back(); stk.pop_back(); onStk[w] = 0;
                            comp[w] = sc;
                            if (w == u) break;
                        }
                        sc++;
                    }
                    callStk.pop_back();
                    if (!callStk.empty()) low[callStk.back()] = min(low[callStk.back()], low[u]);
                }
            }
        }
        value.assign(n, 0);
        for (int i = 0; i < n; i++) {
            if (comp[2 * i] == comp[2 * i + 1]) return false;   // x_i và ¬x_i cùng SCC -> vô nghiệm
            value[i] = comp[2 * i] < comp[2 * i + 1];           // literal ở SCC "muộn" trong tô-pô -> chọn true
        }
        return true;
    }
};

#ifdef CP_DEMO  // g++ -std=c++17 -DCP_DEMO -x c++ two-sat.hpp -o demo && ./demo
int main() {
    TwoSat a(1); a.addFact(0, true); a.addFact(0, false);          // x0 vừa đúng vừa sai -> vô nghiệm
    TwoSat b(2); b.addClause(0, true, 1, true); b.addClause(0, false, 1, false);
    printf("2-SAT: mâu thuẫn thỏa? %d ; hợp lệ thỏa? %d\n", (int)a.solve(), (int)b.solve());   // 0 ; 1
    return 0;
}
#endif
