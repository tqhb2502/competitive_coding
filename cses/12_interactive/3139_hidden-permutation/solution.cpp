#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> buf; // bộ đệm phụ dùng khi trộn (merge)

// Truy vấn judge: trả về true nếu a[i] < a[j].
// endl bắt buộc để flush stdout sau mỗi truy vấn, tránh deadlock.
bool less_than(int i, int j) {
    cout << "? " << i << ' ' << j << endl;
    string resp;
    cin >> resp;
    return resp == "YES";
}

// Sắp xếp idx[l..r) sao cho giá trị ẩn tương ứng tăng dần.
// Phép so sánh giữa hai vị trí được thực hiện bằng truy vấn tới judge.
void merge_sort(vector<int>& idx, int l, int r) {
    if (r - l <= 1) return;
    int m = (l + r) / 2;
    merge_sort(idx, l, m);
    merge_sort(idx, m, r);
    // Trộn hai nửa đã sắp: mỗi bước hỏi đúng một truy vấn so sánh.
    int i = l, j = m, k = l;
    while (i < m && j < r) {
        if (less_than(idx[i], idx[j])) buf[k++] = idx[i++];
        else                           buf[k++] = idx[j++];
    }
    while (i < m) buf[k++] = idx[i++];
    while (j < r) buf[k++] = idx[j++];
    for (int t = l; t < r; ++t) idx[t] = buf[t];
}

int main() {
    if (!(cin >> n)) return 0;

    // Khởi tạo danh sách vị trí 1-based rồi sắp xếp theo giá trị ẩn.
    vector<int> idx(n);
    for (int i = 0; i < n; ++i) idx[i] = i + 1;
    buf.assign(n, 0);

    merge_sort(idx, 0, n);

    // idx[r] là vị trí giữ giá trị nhỏ thứ (r+1) => rank của nó là r+1.
    vector<int> ans(n + 1);
    for (int r = 0; r < n; ++r) ans[idx[r]] = r + 1;

    // In đáp án; endl để flush kết quả cuối cùng.
    cout << '!';
    for (int p = 1; p <= n; ++p) cout << ' ' << ans[p];
    cout << endl;
    return 0;
}
