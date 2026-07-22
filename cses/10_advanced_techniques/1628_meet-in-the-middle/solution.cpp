// Meet in the Middle - CSES 1628
// https://cses.fi/problemset/task/1628
//
// Dem so tap con cua mang n phan tu (n <= 40) co tong bang x.
// Ky thuat: meet in the middle. Chia mang thanh hai nua, sinh tat ca subset sums
// cua moi nua, roi voi moi tong sL cua nua trai dem so tong sR = x - sL o nua phai
// bang binary search tren mang tong da sap xep.

#include <bits/stdc++.h>
using namespace std;

// Sinh tat ca subset sums cua doan a[l..r) vao vector res.
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

    int mid = n / 2;
    vector<long long> L = subsetSums(a, 0, mid);   // nua trai
    vector<long long> R = subsetSums(a, mid, n);   // nua phai

    sort(R.begin(), R.end());

    long long ans = 0;
    for (long long sL : L) {
        long long need = x - sL;
        // Dem so phan tu cua R bang dung need.
        auto range = equal_range(R.begin(), R.end(), need);
        ans += (long long)(range.second - range.first);
    }

    cout << ans << '\n';
    return 0;
}
