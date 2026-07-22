// Hidden Permutation - CSES 3139
// https://cses.fi/problemset/task/3139
//
// Interactive: query "? i j" -> judge answers "YES" if a[i] < a[j] else "NO".
// The comparison "a[i] < a[j]" is a strict total order (a is a permutation),
// so we merge-sort the positions 1..n by their hidden value and assign ranks.
// Merge sort uses at most n*ceil(log2 n) - 2^ceil(log2 n) + 1 = 8977
// comparisons for n = 1000, safely under the 10000 query limit.
// MUST flush stdout after every query (endl) to avoid a deadlock.

#include <bits/stdc++.h>
using namespace std;

int n;
vector<int> buf; // scratch buffer for merging

// Query the judge: returns true if a[i] < a[j].
bool less_than(int i, int j) {
    cout << "? " << i << ' ' << j << endl; // endl -> flush after every query
    string resp;
    cin >> resp;
    return resp == "YES";
}

// Sort idx[l..r) so that hidden values are increasing.
void merge_sort(vector<int>& idx, int l, int r) {
    if (r - l <= 1) return;
    int m = (l + r) / 2;
    merge_sort(idx, l, m);
    merge_sort(idx, m, r);
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

    vector<int> idx(n);
    for (int i = 0; i < n; ++i) idx[i] = i + 1; // 1-based positions
    buf.assign(n, 0);

    merge_sort(idx, 0, n);

    // idx[r] is the position holding the (r+1)-th smallest value.
    vector<int> ans(n + 1);
    for (int r = 0; r < n; ++r) ans[idx[r]] = r + 1;

    cout << '!';
    for (int p = 1; p <= n; ++p) cout << ' ' << ans[p];
    cout << endl; // flush final answer
    return 0;
}
