// Sliding Window Advertisement (CSES 3227)
// https://cses.fi/problemset/task/3227
//
// Với mỗi cửa sổ k tấm ván liên tiếp, đáp án là largest rectangle in histogram
// của đoạn đó. Dùng monotonic stack tính đoạn cực đại (L[j], R[j]) mà a[j] là min,
// rồi công thức:
//     g(s) = max_{j in [s, e]} a[j] * (min(R[j], e) - max(L[j], s) + 1),  e = s+k-1.
// Coi g(s) là hàm theo s: mỗi cột j sinh (tối đa) 4 đoạn-đường thẳng theo s tùy
// biên trái/phải có bị clip hay không. Chèn vào Li Chao tree (segment insert),
// sau đó truy vấn max tại từng s.
//
// Độ phức tạp: O(n log^2 n) thời gian, O(n) bộ nhớ.

#include <bits/stdc++.h>
using namespace std;

struct Line { long long a, b; };            // giá trị = a*s + b
static const long long NEG = -4e18;         // đường "rỗng"

int m;                                       // số cửa sổ = n-k+1, miền vị trí 0..m-1
vector<Line> tr;

// đánh giá đường thẳng tại vị trí pos (0-indexed) ứng với s = pos+1
static inline long long f(const Line& ln, int pos){
    return ln.a * (long long)(pos + 1) + ln.b;
}

// Li Chao: chèn đường nw vào node phủ [lo, hi]
void add(int node, int lo, int hi, Line nw){
    int mid = (lo + hi) >> 1;
    bool lef  = f(nw, lo)  > f(tr[node], lo);
    bool midb = f(nw, mid) > f(tr[node], mid);
    if (midb) swap(tr[node], nw);
    if (lo == hi) return;
    if (lef != midb) add(2*node,   lo,    mid, nw);
    else             add(2*node+1, mid+1, hi,  nw);
}

// chèn đường ln chỉ trên đoạn s in [ql, qr]
void insert_seg(int node, int lo, int hi, int ql, int qr, const Line& ln){
    if (qr < lo || hi < ql) return;
    if (ql <= lo && hi <= qr){ add(node, lo, hi, ln); return; }
    int mid = (lo + hi) >> 1;
    insert_seg(2*node,   lo,    mid, ql, qr, ln);
    insert_seg(2*node+1, mid+1, hi,  ql, qr, ln);
}

// truy vấn max tại điểm pos
long long query(int node, int lo, int hi, int pos){
    long long res = f(tr[node], pos);
    if (lo == hi) return res;
    int mid = (lo + hi) >> 1;
    if (pos <= mid) return max(res, query(2*node,   lo,    mid, pos));
    else            return max(res, query(2*node+1, mid+1, hi,  pos));
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    // previous / next strictly smaller element -> đoạn cực đại [L, R] mà a[j] là min
    vector<int> L(n + 1), R(n + 1);
    vector<int> st;
    st.reserve(n + 1);
    for (int i = 1; i <= n; i++){
        while (!st.empty() && a[st.back()] >= a[i]) st.pop_back();
        L[i] = (st.empty() ? 0 : st.back()) + 1;
        st.push_back(i);
    }
    st.clear();
    for (int i = n; i >= 1; i--){
        while (!st.empty() && a[st.back()] >= a[i]) st.pop_back();
        R[i] = (st.empty() ? (n + 1) : st.back()) - 1;
        st.push_back(i);
    }

    m = n - k + 1;
    tr.assign(4 * m, Line{0, NEG});

    for (int j = 1; j <= n; j++){
        long long aj = a[j];
        int Lj = L[j], Rj = R[j];
        int smin = max(1, j - k + 1);
        int smax = min(j, m);
        if (smin > smax) continue;

        // (1) không clip cả hai: hằng aj*(Rj-Lj+1), s in [Rj-k+1, Lj]
        {
            int lo = max(smin, Rj - k + 1);
            int hi = min(smax, Lj);
            if (lo <= hi){
                Line ln{0, aj * (long long)(Rj - Lj + 1)};
                insert_seg(1, 0, m - 1, lo - 1, hi - 1, ln);
            }
        }
        // (2) chỉ clip trái: aj*(Rj - s + 1) = -aj*s + aj*(Rj+1), s in [max(L+1,R-k+1), smax]
        {
            int lo = max(smin, max(Lj + 1, Rj - k + 1));
            int hi = smax;
            if (lo <= hi){
                Line ln{-aj, aj * (long long)(Rj + 1)};
                insert_seg(1, 0, m - 1, lo - 1, hi - 1, ln);
            }
        }
        // (3) chỉ clip phải: aj*(s + k - Lj) = aj*s + aj*(k-Lj), s in [smin, min(L, R-k)]
        {
            int lo = smin;
            int hi = min(smax, min(Lj, Rj - k));
            if (lo <= hi){
                Line ln{aj, aj * (long long)(k - Lj)};
                insert_seg(1, 0, m - 1, lo - 1, hi - 1, ln);
            }
        }
        // (4) clip cả hai: hằng aj*k, s in [L+1, R-k]
        {
            int lo = max(smin, Lj + 1);
            int hi = min(smax, Rj - k);
            if (lo <= hi){
                Line ln{0, aj * (long long)k};
                insert_seg(1, 0, m - 1, lo - 1, hi - 1, ln);
            }
        }
    }

    string out;
    out.reserve((size_t)m * 7);
    for (int s = 1; s <= m; s++){
        long long v = query(1, 0, m - 1, s - 1);
        out += to_string(v);
        out += (s == m ? '\n' : ' ');
    }
    cout << out;
    return 0;
}
