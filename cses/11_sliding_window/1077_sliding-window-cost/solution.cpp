#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;

    // two multisets: low giữ nửa nhỏ (max của low là median), high giữ nửa lớn.
    // Bất biến: low.size() == high.size() hoặc low.size() == high.size()+1, và
    // mọi phần tử của low <= mọi phần tử của high. sumLow/sumHigh là tổng tích lũy.
    multiset<long long> low, high;
    long long sumLow = 0, sumHigh = 0;

    // rebalance: chuyển phần tử biên giữa low/high cho tới khi phục hồi bất biến.
    auto rebalance = [&]() {
        while (low.size() > high.size() + 1) {
            auto it = prev(low.end());       // max của low
            long long v = *it;
            sumLow -= v; low.erase(it);
            high.insert(v); sumHigh += v;
        }
        while (low.size() < high.size()) {
            auto it = high.begin();          // min của high
            long long v = *it;
            sumHigh -= v; high.erase(it);
            low.insert(v); sumLow += v;
        }
    };

    // add: chèn x vào low nếu x <= median hiện tại, ngược lại vào high.
    auto add = [&](long long x) {
        if (low.empty() || x <= *low.rbegin()) {
            low.insert(x); sumLow += x;
        } else {
            high.insert(x); sumHigh += x;
        }
        rebalance();
    };

    // remove: tìm x trong low trước, nếu không có thì trong high, rồi xóa một bản.
    auto remove = [&](long long x) {
        auto it = low.find(x);
        if (it != low.end()) {
            sumLow -= x; low.erase(it);
        } else {
            it = high.find(x);
            sumHigh -= x; high.erase(it);
        }
        rebalance();
    };

    vector<long long> ans;
    ans.reserve(n - k + 1);

    // Trượt cửa sổ: thêm phần tử phải, khi đủ k phần tử thì tính chi phí rồi bỏ trái.
    for (int i = 0; i < n; i++) {
        add(a[i]);
        if (i >= k - 1) {
            long long m = *low.rbegin();     // median = phần tử lớn nhất của low
            // cost = sum (m - a_i) trên low + sum (a_i - m) trên high = sum |a_i - m|
            long long cost = (m * (long long)low.size() - sumLow)
                           + (sumHigh - m * (long long)high.size());
            ans.push_back(cost);
            remove(a[i - k + 1]);            // bỏ phần tử trái khỏi cửa sổ
        }
    }

    string out;
    out.reserve(ans.size() * 12);
    for (size_t i = 0; i < ans.size(); i++) {
        if (i) out.push_back(' ');
        out += to_string(ans[i]);
    }
    out.push_back('\n');
    cout << out;
    return 0;
}
