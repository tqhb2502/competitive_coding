#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // cnt[value] = tần suất của value trong cửa sổ hiện tại; distinct = số giá trị
    // đang có tần suất lớn hơn 0. Reserve để giảm số lần rehash của hash map.
    unordered_map<int, int> cnt;
    cnt.reserve(n * 2);
    cnt.max_load_factor(0.7f);

    int distinct = 0;

    // Khởi tạo cửa sổ đầu tiên [0, k-1]: mỗi giá trị lần đầu xuất hiện làm distinct tăng 1.
    for (int i = 0; i < k; ++i) {
        if (cnt[a[i]]++ == 0) ++distinct;
    }

    string out;
    out.reserve((size_t)(n - k + 1) * 7);

    // Ghi nhanh một số nguyên vào chuỗi kết quả (tránh chi phí của luồng xuất chuẩn).
    auto append_int = [&](int v) {
        if (v == 0) { out.push_back('0'); return; }
        char buf[12];
        int len = 0;
        while (v > 0) { buf[len++] = char('0' + v % 10); v /= 10; }
        while (len > 0) out.push_back(buf[--len]);
    };

    append_int(distinct);

    // Trượt cửa sổ từ trái sang phải, mỗi bước cập nhật rồi ghi lại distinct.
    for (int i = k; i < n; ++i) {
        // Thêm phần tử a[i] vào cửa sổ: nếu giá trị này chưa có thì distinct tăng 1.
        if (cnt[a[i]]++ == 0) ++distinct;
        // Bỏ phần tử bên trái a[i-k]: nếu tần suất về 0 thì distinct giảm 1 và xóa khỏi map.
        int y = a[i - k];
        if (--cnt[y] == 0) {
            --distinct;
            cnt.erase(y);
        }
        out.push_back(' ');
        append_int(distinct);
    }
    out.push_back('\n');

    fputs(out.c_str(), stdout);
    return 0;
}
