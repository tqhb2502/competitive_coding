// Colored Chairs - CSES 3273
// https://cses.fi/problemset/task/3273
//
// Interactive: n ghe (n LE) xep thanh vong tron, moi ghe mau R hoac B.
// Tim hai ghe ke nhau cung mau. Query "? i" -> judge tra 'R'/'B'.
// Tra loi "! i" nghia la ghe i va i+1 (n+1 = 1) cung mau. Toi da 20 query.
//
// Chien luoc: query mau ghe 1 va ghe n. Neu bang nhau -> canh vong (n,1) => "! n".
// Nguoc lai binary search tren duong thang [1,n] voi bat bien "inconsistent voi
// cach to xen ke" (chac chan chua mot cap ke cung mau ben trong).
// Chi phi: 2 + ceil(log2(n-1)) <= 2 + 18 = 20 query.

#include <bits/stdc++.h>
using namespace std;

// Query mau cua ghe i; endl flush stdout sau moi query.
char ask(int i) {
    cout << "? " << i << endl;   // endl -> flush, bat buoc cho interactive
    char c;
    cin >> c;                    // doc 'R' hoac 'B' (cin bo qua khoang trang)
    return c;
}

int main() {
    int n;
    cin >> n;

    char c1 = ask(1);
    char cn = ask(n);

    // (n-1) chan (n le) => xen ke du doan color(1)==color(n).
    // Neu that su bang nhau => ghe 1 va ghe n ke qua canh vong, cung mau.
    if (c1 == cn) {
        cout << "! " << n << endl;
        return 0;
    }

    // c1 != cn => doan [1,n] inconsistent => binary search tim cap ke cung mau.
    int lo = 1, hi = n;
    char clo = c1, chi = cn;
    (void)chi; // chi khong bat buoc dung, giu lai cho ro rang

    while (hi - lo > 1) {
        int mid = (lo + hi) / 2;
        char cm = ask(mid);
        // [lo,mid] nhat quan (consistent) khi: (clo==cm) == ((mid-lo) chan)
        bool consistentLeft = ((clo == cm) == (((mid - lo) % 2) == 0));
        if (!consistentLeft) {
            // nua trai inconsistent -> thu hep ve [lo,mid]
            hi = mid;
            chi = cm;
        } else {
            // nua trai consistent -> nua phai [mid,hi] chac chan inconsistent
            lo = mid;
            clo = cm;
        }
    }

    // hi - lo == 1 va [lo,hi] inconsistent => color(lo)==color(hi)
    // => ghe lo va lo+1 cung mau.
    cout << "! " << lo << endl;
    return 0;
}
