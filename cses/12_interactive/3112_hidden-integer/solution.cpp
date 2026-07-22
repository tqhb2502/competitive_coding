// Hidden Integer - CSES 3112
// https://cses.fi/problemset/task/3112
//
// Interactive: judge giau so nguyen x (1 <= x <= 10^9).
// Query "? y": tra "YES" neu y < x, "NO" neu y >= x. Tra loi cuoi: "! x".
// Toi da 30 query. Dung binary search theo gia tri: ceil(log2(10^9)) = 30.
// Flush sau moi query bang endl.

#include <bits/stdc++.h>
using namespace std;

int main() {
    long long lo = 1, hi = 1000000000LL; // bat bien: lo <= x <= hi
    while (lo < hi) {
        long long mid = (lo + hi) / 2; // 1 <= mid <= hi-1, query hop le
        cout << "? " << mid << endl;   // endl flush stdout
        string resp;
        if (!(cin >> resp)) return 0;  // judge dong stream -> thoat
        if (resp == "YES") {
            // mid < x  => x >= mid + 1
            lo = mid + 1;
        } else {
            // NO: x <= mid
            hi = mid;
        }
    }
    cout << "! " << lo << endl; // lo == hi == x
    return 0;
}
