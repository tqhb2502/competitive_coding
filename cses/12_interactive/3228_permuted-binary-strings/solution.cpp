// Permuted Binary Strings — CSES 3228
// https://cses.fi/problemset/task/3228
//
// Interactive: hidden permutation a[1..n]. A query "? b_1..b_n" returns
// "b_{a_1}..b_{a_n}", i.e. response position i is the bit of the sent string
// at index a_i. Limit: 10 queries; 1 <= n <= 1000.
//
// Strategy: since n <= 1000 < 2^10, encode each a_i by its bits. For query j,
// set b_k = bit j of k. Response position i then reveals bit j of a_i.
// After B = ceil(log2(n+1)) <= 10 queries we reconstruct every a_i.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (!(cin >> n)) return 0;

    // Smallest B with 2^B > n  ->  B bits can represent every value in [1..n].
    int B = 0;
    while ((1 << B) <= n) B++;   // for n <= 1000, B <= 10

    vector<int> a(n + 1, 0);     // 1-indexed hidden permutation

    for (int j = 0; j < B; j++) {
        // Build query: b_k = bit j of k, for k = 1..n
        string query;
        query.reserve(n);
        for (int k = 1; k <= n; k++)
            query.push_back(((k >> j) & 1) ? '1' : '0');

        cout << "? " << query << endl;   // endl flushes stdout

        string resp;
        cin >> resp;                     // resp[i-1] = bit j of a_i
        for (int i = 1; i <= n; i++)
            if (resp[i - 1] == '1') a[i] |= (1 << j);
    }

    cout << "!";
    for (int i = 1; i <= n; i++) cout << ' ' << a[i];
    cout << endl;                        // flush final answer
    return 0;
}
