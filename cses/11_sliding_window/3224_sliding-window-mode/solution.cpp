// Sliding Window Mode - CSES 3224
// https://cses.fi/problemset/task/3224
//
// For each window of k consecutive elements, print the mode (most frequent
// value); ties broken by the smallest value.
//
// Approach: coordinate-compress values, keep cnt[v] per value and freqSet[f]
// (a std::set of compressed ids that currently have frequency f). maxFreq is
// the largest non-empty frequency level; the answer is the smallest id at that
// level. Each add/remove changes one frequency by 1, so maxFreq moves by at
// most 1 per operation. Total time O(n log n).

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    // Nén tọa độ: giữ nguyên thứ tự -> id nhỏ hơn <=> giá trị nhỏ hơn.
    vector<int> vals(a);
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int m = (int)vals.size();
    vector<int> id(n);
    for (int i = 0; i < n; i++)
        id[i] = int(lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin());

    vector<int> cnt(m, 0);          // tần suất của từng id nén trong cửa sổ
    vector<set<int>> freqSet(k + 1); // freqSet[f] = tập id có tần suất đúng bằng f
    int maxFreq = 0;

    auto addElem = [&](int x) {
        int c = cnt[x];
        if (c > 0) freqSet[c].erase(x);
        c++;
        cnt[x] = c;
        freqSet[c].insert(x);
        if (c > maxFreq) maxFreq = c;
    };
    auto removeElem = [&](int x) {
        int c = cnt[x];
        freqSet[c].erase(x);
        c--;
        cnt[x] = c;
        if (c > 0) freqSet[c].insert(x);
        if (freqSet[maxFreq].empty()) maxFreq--; // giảm nhiều nhất 1 đơn vị
    };

    // Dựng cửa sổ đầu tiên gồm k phần tử.
    for (int i = 0; i < k; i++) addElem(id[i]);

    string out;
    out.reserve((size_t)(n - k + 1) * 11);
    out += to_string(vals[*freqSet[maxFreq].begin()]);

    for (int i = k; i < n; i++) {
        removeElem(id[i - k]);
        addElem(id[i]);
        out += ' ';
        out += to_string(vals[*freqSet[maxFreq].begin()]);
    }
    out += '\n';

    cout << out;
    return 0;
}
