#include <bits/stdc++.h>
using namespace std;

const int MAXN = 200005;

// Implicit treap lưu bằng các mảng song song; node 0 đóng vai trò NULL.
int lc[MAXN], rc[MAXN], sz[MAXN];   // con trái, con phải, size của cây con
unsigned long long pri[MAXN];       // priority ngẫu nhiên để giữ max-heap
char val[MAXN];                     // ký tự tại mỗi node
int cnt = 0;

// Priority lấy từ mt19937_64 để gần như không trùng, tránh cây bị lệch.
mt19937_64 rng(
    (unsigned long long)chrono::steady_clock::now().time_since_epoch().count());

// Tạo node mới giữ một ký tự, size = 1.
inline int newNode(char c) {
    int id = ++cnt;
    lc[id] = rc[id] = 0;
    sz[id] = 1;
    pri[id] = rng();
    val[id] = c;
    return id;
}

// Cập nhật lại size của node t từ hai cây con.
inline void update(int t) {
    if (t) sz[t] = sz[lc[t]] + sz[rc[t]] + 1;
}

// Tách t thành (a, b): a giữ k phần tử đầu theo in-order, b giữ phần còn lại.
// Tại t, nếu size con trái < k thì t thuộc a và đi tiếp sang con phải.
void split(int t, int k, int &a, int &b) {
    if (!t) { a = b = 0; return; }
    if (sz[lc[t]] < k) {
        a = t;
        split(rc[t], k - sz[lc[t]] - 1, rc[t], b);
        update(t);
    } else {
        b = t;
        split(lc[t], k, a, lc[t]);
        update(t);
    }
}

// Nối cây a đứng trước cây b; chọn gốc là node có priority lớn hơn để giữ max-heap.
int merge(int a, int b) {
    if (!a) return b;
    if (!b) return a;
    if (pri[a] > pri[b]) {
        rc[a] = merge(rc[a], b);
        update(a);
        return a;
    } else {
        lc[b] = merge(a, lc[b]);
        update(b);
        return b;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    string s;
    cin >> s;

    // Xây cây ban đầu bằng cách merge từng ký tự vào cuối.
    int root = 0;
    for (int i = 0; i < n; ++i)
        root = merge(root, newNode(s[i]));

    // Mỗi thao tác cắt đoạn [a, b] ra rồi dán vào cuối: chỉ 2 split + 2 merge.
    while (m--) {
        int a, b;
        cin >> a >> b;
        int T1, R, A, M;
        split(root, b, T1, R);      // T1 = [1..b], R = [b+1..n]
        split(T1, a - 1, A, M);     // A = [1..a-1], M = [a..b] (đoạn bị cắt)
        root = merge(merge(A, R), M); // xâu mới = A + R + M
    }

    // Duyệt in-order lặp (stack thủ công) để lấy xâu kết quả, tránh tràn stack.
    string res;
    res.reserve(cnt);
    static int st[MAXN];
    int top = 0, cur = root;
    while (cur || top) {
        while (cur) { st[top++] = cur; cur = lc[cur]; }
        cur = st[--top];
        res.push_back(val[cur]);
        cur = rc[cur];
    }

    cout << res << '\n';
    return 0;
}
