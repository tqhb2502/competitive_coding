#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;

// Mỗi node của implicit treap: con trái/phải, size cây con, priority ngẫu
// nhiên, ký tự val và cờ lazy rev_ (đánh dấu đoạn cần đảo ngược).
int lc[MAXN], rc[MAXN], sz[MAXN];
unsigned int pri[MAXN];
char val[MAXN];
bool rev_[MAXN];
int nodeCount = 0;

mt19937 rng(1234567891u);

// Tạo một node mới ứng với ký tự c.
int newNode(char c) {
    int x = ++nodeCount;
    lc[x] = rc[x] = 0;
    sz[x] = 1;
    pri[x] = rng();
    val[x] = c;
    rev_[x] = false;
    return x;
}

// Cập nhật lại size của node từ hai cây con.
inline void update(int x) {
    if (!x) return;
    sz[x] = 1 + sz[lc[x]] + sz[rc[x]];
}

// Đẩy cờ đảo ngược xuống: hoán đổi con trái/phải rồi bật cờ cho hai con.
inline void pushDown(int x) {
    if (x && rev_[x]) {
        swap(lc[x], rc[x]);
        if (lc[x]) rev_[lc[x]] ^= 1;
        if (rc[x]) rev_[rc[x]] ^= 1;
        rev_[x] = false;
    }
}

// split: tách t sao cho k node đầu (theo in-order) về l, phần còn lại về r.
void split(int t, int k, int &l, int &r) {
    if (!t) { l = r = 0; return; }
    pushDown(t);
    if (sz[lc[t]] < k) {
        // Đủ node bên trái + gốc: đi tiếp sang cây con phải.
        int rr;
        split(rc[t], k - sz[lc[t]] - 1, rc[t], rr);
        l = t;
        r = rr;
    } else {
        // Cắt nằm trong cây con trái.
        int ll;
        split(lc[t], k, ll, lc[t]);
        l = ll;
        r = t;
    }
    update(t);
}

// merge: ghép cây l (đứng trước) với cây r (đứng sau) theo quy ước max-heap.
int merge(int l, int r) {
    if (!l || !r) return l | r;
    if (pri[l] > pri[r]) {
        pushDown(l);
        rc[l] = merge(rc[l], r);
        update(l);
        return l;
    } else {
        pushDown(r);
        lc[r] = merge(l, lc[r]);
        update(r);
        return r;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    string s;
    cin >> s;

    // Xây treap ban đầu bằng cách merge lần lượt từng ký tự.
    int root = 0;
    for (int i = 0; i < n; ++i)
        root = merge(root, newNode(s[i]));

    for (int q = 0; q < m; ++q) {
        int a, b;
        cin >> a >> b;
        int A, B, C, tmp;
        // Tách thành A = [1, a-1] và tmp = [a, n].
        split(root, a - 1, A, tmp);
        // Tách tmp thành B = [a, b] và C = [b+1, n].
        split(tmp, b - a + 1, B, C);
        // Đảo ngược đoạn giữa bằng cách bật cờ lazy ở gốc B, rồi ghép lại.
        rev_[B] ^= 1;
        root = merge(merge(A, B), C);
    }

    // Duyệt in-order bằng vòng lặp với stack tường minh (đẩy pushDown khi đi
    // xuống) để thu về xâu kết quả mà không dùng đệ quy sâu.
    string out;
    out.reserve(n);
    vector<int> stk;
    stk.reserve(64);
    int cur = root;
    while (cur || !stk.empty()) {
        while (cur) {
            pushDown(cur);
            stk.push_back(cur);
            cur = lc[cur];
        }
        cur = stk.back(); stk.pop_back();
        out.push_back(val[cur]);
        cur = rc[cur];
    }

    cout << out << '\n';
    return 0;
}
