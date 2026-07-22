// Sliding Window Inversions - CSES 3223
// https://cses.fi/problemset/task/3223
//
// Duy trì số nghịch thế (inversion) của cửa sổ kích thước k khi trượt bằng
// Fenwick tree (BIT) trên giá trị đã nén tọa độ. Khi bỏ phần tử trái nhất x,
// trừ đi số phần tử < x; khi thêm phần tử phải mới y, cộng thêm số phần tử > y.
// Độ phức tạp O(n log n).

#include <bits/stdc++.h>
using namespace std;

int m;                 // số giá trị phân biệt sau khi nén
vector<int> bit;       // Fenwick tree lưu số lượng từng giá trị trong cửa sổ

void update(int i, int delta) {
    for (; i <= m; i += i & (-i)) bit[i] += delta;
}
long long query(int i) {           // số phần tử có chỉ số nén <= i
    long long s = 0;
    for (; i > 0; i -= i & (-i)) s += bit[i];
    return s;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> a(n);
    vector<int> vals(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        vals[i] = a[i];
    }

    // Nén tọa độ
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    m = (int)vals.size();

    vector<int> c(n);
    for (int i = 0; i < n; i++)
        c[i] = (int)(lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin()) + 1;

    bit.assign(m + 1, 0);

    vector<long long> ans;
    ans.reserve(n - k + 1);

    long long inv = 0;

    // Xây cửa sổ đầu tiên [0, k-1]
    for (int i = 0; i < k; i++) {
        // số phần tử đã có mà LỚN HƠN c[i] = (số phần tử hiện có) - (số <= c[i])
        inv += (long long)i - query(c[i]);
        update(c[i], 1);
    }
    ans.push_back(inv);

    // Trượt cửa sổ
    for (int i = k; i < n; i++) {
        int L = i - k;                       // phần tử trái nhất bị bỏ
        inv -= query(c[L] - 1);              // số phần tử < c[L] trong cửa sổ
        update(c[L], -1);

        // Bây giờ BIT còn k-1 phần tử; số phần tử > c[i]
        inv += (long long)(k - 1) - query(c[i]);
        update(c[i], 1);

        ans.push_back(inv);
    }

    // Xuất kết quả trên một dòng, cách nhau bởi dấu cách
    string out;
    out.reserve(ans.size() * 6);
    for (size_t i = 0; i < ans.size(); i++) {
        if (i) out.push_back(' ');
        out += to_string(ans[i]);
    }
    out.push_back('\n');
    cout << out;

    return 0;
}
