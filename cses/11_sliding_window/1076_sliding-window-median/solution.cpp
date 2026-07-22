// Sliding Window Median (CSES 1076)
// https://cses.fi/problemset/task/1076
//
// Median của mỗi cửa sổ k phần tử. Khi k chẵn lấy phần tử NHỎ HƠN trong hai
// phần tử giữa (lower median) -> chính là phần tử ở hạng (k-1)/2 (0-indexed).
// Dùng hai multiset: low (nửa dưới) và high (nửa trên) với bất biến
// max(low) <= min(high) và |low| = target = (k+1)/2; đáp án = *low.rbegin().
// Độ phức tạp: O(n log k) thời gian, O(k) bộ nhớ.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> a(n);
    for (auto &x : a) cin >> x;

    multiset<int> low, high;      // low giữ nửa dưới, đáp án = *low.rbegin()
    const int target = (k + 1) / 2;   // kích thước mong muốn của low

    auto rebalance = [&]() {
        // Chuyển bớt từ low sang high nếu low quá lớn.
        while ((int)low.size() > target) {
            auto it = prev(low.end());
            high.insert(*it);
            low.erase(it);
        }
        // Kéo từ high sang low nếu low còn thiếu (bảo vệ high không rỗng,
        // hữu ích trong giai đoạn nạp đủ k phần tử đầu tiên).
        while ((int)low.size() < target && !high.empty()) {
            auto it = high.begin();
            low.insert(*it);
            high.erase(it);
        }
    };

    auto add = [&](int x) {
        if (low.empty() || x <= *low.rbegin()) low.insert(x);
        else high.insert(x);
    };

    auto remove = [&](int x) {
        // low luôn khác rỗng tại thời điểm gọi remove.
        if (x <= *low.rbegin()) low.erase(low.find(x));
        else high.erase(high.find(x));
    };

    vector<int> ans;
    ans.reserve(n - k + 1);

    for (int i = 0; i < n; i++) {
        add(a[i]);
        if (i >= k) remove(a[i - k]);   // bỏ phần tử rời khỏi cửa sổ
        rebalance();
        if (i >= k - 1) ans.push_back(*low.rbegin());
    }

    // In n-k+1 giá trị, cách nhau bởi dấu cách.
    string out;
    out.reserve(ans.size() * 11);
    for (size_t i = 0; i < ans.size(); i++) {
        if (i) out.push_back(' ');
        out += to_string(ans[i]);
    }
    out.push_back('\n');
    fputs(out.c_str(), stdout);
    return 0;
}
