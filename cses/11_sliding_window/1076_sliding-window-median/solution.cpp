#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<int> a(n);
    for (auto &x : a) cin >> x;

    // Hai multiset cân bằng: low giữ nửa dưới (đáp án = *low.rbegin() = max của low),
    // high giữ nửa trên; bất biến max(low) <= min(high) và |low| = target.
    multiset<int> low, high;
    const int target = (k + 1) / 2;   // kích thước mong muốn của low (đáp án ở hạng (k-1)/2)

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

    // Thêm x vào nửa dưới nếu low rỗng hoặc x <= max(low), ngược lại thêm vào nửa trên.
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

    // Trượt cửa sổ: mỗi bước thêm phần tử mới, bỏ phần tử rời cửa sổ, cân bằng lại
    // rồi ghi median khi cửa sổ đã đủ k phần tử.
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
