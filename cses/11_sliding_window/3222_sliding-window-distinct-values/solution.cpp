// Sliding Window Distinct Values
// https://cses.fi/problemset/task/3222
//
// Với mỗi cửa sổ k phần tử liên tiếp, đếm số giá trị phân biệt (distinct).
// Kỹ thuật: sliding window + hashmap counts, duy trì biến distinct.
// Khi thêm x: nếu cnt[x]==0 thì distinct++, rồi cnt[x]++.
// Khi bỏ y: cnt[y]-- ; nếu cnt[y]==0 thì distinct--.
// Thời gian O(n) trung bình, bộ nhớ O(n).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    unordered_map<int, int> cnt;
    cnt.reserve(n * 2);
    cnt.max_load_factor(0.7f);

    int distinct = 0;

    // Khởi tạo cửa sổ đầu tiên [0, k-1].
    for (int i = 0; i < k; ++i) {
        if (cnt[a[i]]++ == 0) ++distinct;
    }

    string out;
    out.reserve((size_t)(n - k + 1) * 7);

    auto append_int = [&](int v) {
        if (v == 0) { out.push_back('0'); return; }
        char buf[12];
        int len = 0;
        while (v > 0) { buf[len++] = char('0' + v % 10); v /= 10; }
        while (len > 0) out.push_back(buf[--len]);
    };

    append_int(distinct);

    for (int i = k; i < n; ++i) {
        // Thêm a[i].
        if (cnt[a[i]]++ == 0) ++distinct;
        // Bỏ a[i-k].
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
