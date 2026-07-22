// Sliding Window Mex - CSES 3219
// https://cses.fi/problemset/task/3219
//
// Ý tưởng: mex của mỗi cửa sổ k phần tử luôn nằm trong [0, k]
// (không thể >= k+1 vì cần k+1 giá trị phân biệt 0..k nhưng cửa sổ chỉ có k
// phần tử). Ta theo dõi count của các giá trị trong [0, k] bằng segment tree
// lưu min-count; mex = chỉ số nhỏ nhất có count == 0, tìm bằng descend O(log k).
// Các giá trị > k bị bỏ qua vì không ảnh hưởng tới mex.
// Tổng thể: O(n log k) thời gian, O(k) bộ nhớ.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (auto &x : a) cin >> x;

    // Chỉ quan tâm các giá trị 0..k (m = k+1 giá trị).
    const int m = k + 1;
    int sz = 1;
    while (sz < m) sz <<= 1;

    const int INF = INT_MAX / 2;
    vector<int> tree(2 * sz, 0);   // count = 0 cho các lá hợp lệ
    for (int i = m; i < sz; ++i) tree[sz + i] = INF; // lá thừa -> +vô cực
    for (int i = sz - 1; i >= 1; --i) tree[i] = min(tree[2 * i], tree[2 * i + 1]);

    auto update = [&](int pos, int delta) {
        int i = pos + sz;
        tree[i] += delta;
        for (i >>= 1; i >= 1; i >>= 1)
            tree[i] = min(tree[2 * i], tree[2 * i + 1]);
    };

    // mex = chỉ số nhỏ nhất có count == 0.
    auto query_mex = [&]() -> int {
        int i = 1;
        while (i < sz) i = (tree[2 * i] == 0) ? (2 * i) : (2 * i + 1);
        return i - sz;
    };

    // val > k thì trả về -1 để bỏ qua.
    auto val = [&](int x) -> int { return x > k ? -1 : x; };

    vector<int> ans;
    ans.reserve(n - k + 1);

    for (int i = 0; i < k; ++i) {
        int v = val(a[i]);
        if (v >= 0) update(v, +1);
    }
    ans.push_back(query_mex());

    for (int i = k; i < n; ++i) {
        int add = val(a[i]);
        if (add >= 0) update(add, +1);
        int rem = val(a[i - k]);
        if (rem >= 0) update(rem, -1);
        ans.push_back(query_mex());
    }

    string out;
    out.reserve(ans.size() * 3);
    for (size_t i = 0; i < ans.size(); ++i) {
        if (i) out.push_back(' ');
        out += to_string(ans[i]);
    }
    out.push_back('\n');
    cout << out;
    return 0;
}
