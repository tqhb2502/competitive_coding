#include <bits/stdc++.h>
using namespace std;

// Sinh tất cả subset sums (tổng các tập con) của đoạn a[l..r) vào vector kết quả.
// Nhân đôi dần: bắt đầu từ {0}, mỗi phần tử mới cộng vào toàn bộ các tổng đã có.
static vector<long long> subsetSums(const vector<long long>& a, int l, int r) {
    int k = r - l;
    vector<long long> res;
    res.reserve(size_t(1) << k);
    res.push_back(0);
    for (int i = 0; i < k; ++i) {
        long long v = a[l + i];
        int cur = (int)res.size();
        for (int j = 0; j < cur; ++j) res.push_back(res[j] + v);
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x;
    if (!(cin >> n >> x)) return 0;

    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    // Chia mảng thành hai nửa và sinh subset sums cho từng nửa.
    int mid = n / 2;
    vector<long long> L = subsetSums(a, 0, mid);   // nửa trái
    vector<long long> R = subsetSums(a, mid, n);   // nửa phải

    // Sắp xếp nửa phải để tra cứu nhanh bằng binary search.
    sort(R.begin(), R.end());

    // Với mỗi tổng sL của nửa trái, đếm số tổng sR = x - sL trong nửa phải.
    long long ans = 0;
    for (long long sL : L) {
        long long need = x - sL;
        auto range = equal_range(R.begin(), R.end(), need);
        ans += (long long)(range.second - range.first);
    }

    cout << ans << '\n';
    return 0;
}
