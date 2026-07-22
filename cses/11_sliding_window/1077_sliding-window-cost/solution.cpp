// Sliding Window Cost - CSES 1077
// https://cses.fi/problemset/task/1077
//
// Với mỗi cửa sổ k phần tử, chi phí tối thiểu để đưa về cùng một giá trị là
// sum |a_i - median|. Dùng two multisets (low/high) cân bằng để duy trì median
// và hai biến sum tích lũy, cập nhật O(log k) khi cửa sổ trượt.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<long long> a(n);
    for (auto &x : a) cin >> x;

    // low: nửa nhỏ (max của low là median); high: nửa lớn.
    // Bất biến: low.size() == high.size() hoặc low.size() == high.size()+1,
    // và mọi phần tử low <= mọi phần tử high.
    multiset<long long> low, high;
    long long sumLow = 0, sumHigh = 0;

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

    auto add = [&](long long x) {
        if (low.empty() || x <= *low.rbegin()) {
            low.insert(x); sumLow += x;
        } else {
            high.insert(x); sumHigh += x;
        }
        rebalance();
    };

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

    for (int i = 0; i < n; i++) {
        add(a[i]);
        if (i >= k - 1) {
            long long m = *low.rbegin();     // median
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
