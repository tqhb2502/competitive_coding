// Hamming Distance - CSES 2136
// https://cses.fi/problemset/task/2136
//
// n bit strings (n <= 2e4), each of length k <= 30.
// Find the minimum Hamming distance over all pairs.
//
// Since k <= 30, each string fits in a 32-bit unsigned integer.
// Hamming(a, b) = popcount(a XOR b). We brute force all O(n^2)
// pairs; each pair costs a single XOR + hardware popcount, so
// ~2e8 fast operations fit comfortably in the time limit.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, k;
    if (scanf("%d %d", &n, &k) != 2) return 0;

    vector<unsigned int> a(n);
    char buf[40];
    for (int i = 0; i < n; i++) {
        scanf("%s", buf);
        unsigned int x = 0;
        for (int j = 0; j < k; j++)
            x = (x << 1) | (unsigned int)(buf[j] - '0');
        a[i] = x;
    }

    int ans = k;                       // upper bound: strings differ in at most k bits
    for (int i = 0; i < n && ans > 0; i++) {
        unsigned int ai = a[i];
        for (int j = i + 1; j < n; j++) {
            int d = __builtin_popcount(ai ^ a[j]);
            if (d < ans) {
                ans = d;
                if (ans == 0) break;   // cannot get smaller
            }
        }
    }

    printf("%d\n", ans);
    return 0;
}
