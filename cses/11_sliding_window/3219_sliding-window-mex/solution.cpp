#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (auto &x : a) cin >> x;

    // Chỉ quan tâm các giá trị 0..k (m = k+1 giá trị vì mex luôn thuộc [0, k]).
    const int m = k + 1;
    int sz = 1;
    while (sz < m) sz <<= 1;

    // Segment tree trên miền giá trị: mỗi lá giữ count của một giá trị,
    // node trong giữ min của các count.
    const int INF = INT_MAX / 2;
    vector<int> tree(2 * sz, 0);   // count = 0 cho các lá hợp lệ
    for (int i = m; i < sz; ++i) tree[sz + i] = INF; // lá thừa (ngoài [0, k]) -> +vô cực
    for (int i = sz - 1; i >= 1; --i) tree[i] = min(tree[2 * i], tree[2 * i + 1]);

    // Cộng delta vào count của một giá trị rồi cập nhật min dọc lên gốc.
    auto update = [&](int pos, int delta) {
        int i = pos + sz;
        tree[i] += delta;
        for (i >>= 1; i >= 1; i >>= 1)
            tree[i] = min(tree[2 * i], tree[2 * i + 1]);
    };

    // mex là chỉ số nhỏ nhất có count == 0: descend chọn nhánh trái nhất có min == 0.
    auto query_mex = [&]() -> int {
        int i = 1;
        while (i < sz) i = (tree[2 * i] == 0) ? (2 * i) : (2 * i + 1);
        return i - sz;
    };

    // Giá trị > k không ảnh hưởng tới mex nên trả về -1 để bỏ qua.
    auto val = [&](int x) -> int { return x > k ? -1 : x; };

    vector<int> ans;
    ans.reserve(n - k + 1);

    // Nạp cửa sổ đầu tiên gồm k phần tử.
    for (int i = 0; i < k; ++i) {
        int v = val(a[i]);
        if (v >= 0) update(v, +1);
    }
    ans.push_back(query_mex());

    // Trượt cửa sổ: thêm phần tử mới, bỏ phần tử cũ, rồi truy vấn mex.
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
