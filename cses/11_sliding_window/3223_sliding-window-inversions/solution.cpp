#include <bits/stdc++.h>
using namespace std;

int m;                 // số giá trị phân biệt sau khi nén tọa độ
vector<int> bit;       // Fenwick tree (BIT) lưu số lượng từng giá trị trong cửa sổ

// Cộng delta vào vị trí i của Fenwick tree
void update(int i, int delta) {
    for (; i <= m; i += i & (-i)) bit[i] += delta;
}
// Trả về số phần tử có chỉ số nén <= i đang nằm trong cửa sổ
long long query(int i) {
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

    // Nén tọa độ các giá trị về khoảng 1..m
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

    // Xây cửa sổ đầu tiên [0, k-1] bằng cách thêm dần từng phần tử vào bên phải
    for (int i = 0; i < k; i++) {
        // Số phần tử đã có mà LỚN HƠN c[i] = (số phần tử hiện có) - (số <= c[i])
        inv += (long long)i - query(c[i]);
        update(c[i], 1);
    }
    ans.push_back(inv);

    // Trượt cửa sổ sang phải từng bước
    for (int i = k; i < n; i++) {
        int L = i - k;                       // phần tử trái nhất bị bỏ khỏi cửa sổ
        // Bỏ phần tử trái: trừ đi số phần tử nhỏ hơn nó trong cửa sổ
        inv -= query(c[L] - 1);
        update(c[L], -1);

        // Thêm phần tử phải: BIT còn k-1 phần tử, cộng số phần tử lớn hơn c[i]
        inv += (long long)(k - 1) - query(c[i]);
        update(c[i], 1);

        ans.push_back(inv);
    }

    // Xuất kết quả trên một dòng, các giá trị cách nhau bởi dấu cách
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
