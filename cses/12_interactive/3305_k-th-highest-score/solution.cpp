// K-th Highest Score — CSES 3305 (Interactive)
// https://cses.fi/problemset/task/3305
//
// Tim diem cao thu k trong hop cua hai mang giam dan F[1..n], S[1..n].
// Query: "F i" -> F[i], "S i" -> S[i]. Tra loi: "! s". Toi da 100 query.
//
// Chien luoc: binary search partition i + j = k (i lay tu F, j tu S).
// Partition dung <=> F[i] > S[j+1] va S[j] > F[i+1]; dap an = min(F[i], S[j]).
// Bien ao: F[0]=S[0]=+INF, F[n+1]=S[n+1]=-INF. Cache moi gia tri da hoi.
// So query: <= ~17 vong * 4 = ~68 < 100. Flush sau moi query (dung endl).

#include <bits/stdc++.h>
using namespace std;

static const long long POSINF = (long long)4e18;
static const long long NEGINF = (long long)-4e18;

int n;
long long k;
vector<long long> fval, sval;
vector<char> fq, sq;

// Gui mot query va doc gia tri tra ve (endl -> flush stdout).
long long query(char c, int i) {
    cout << c << ' ' << i << endl; // endl bao dam flush
    long long x;
    cin >> x;
    return x;
}

// F[i] voi bien ao va cache. i<=0 -> +INF, i>n -> -INF.
long long getF(int i) {
    if (i <= 0) return POSINF;
    if (i > n)  return NEGINF;
    if (!fq[i]) { fval[i] = query('F', i); fq[i] = 1; }
    return fval[i];
}

// S[i] voi bien ao va cache.
long long getS(int i) {
    if (i <= 0) return POSINF;
    if (i > n)  return NEGINF;
    if (!sq[i]) { sval[i] = query('S', i); sq[i] = 1; }
    return sval[i];
}

int main() {
    if (!(cin >> n >> k)) return 0;

    fval.assign(n + 2, 0);
    sval.assign(n + 2, 0);
    fq.assign(n + 2, 0);
    sq.assign(n + 2, 0);

    int lo = (int)max(0LL, k - n);
    int hi = (int)min(k, (long long)n);
    long long ans = -1;

    while (lo <= hi) {
        int i = lo + (hi - lo) / 2;
        long long j = k - i;

        long long Fi  = getF(i);       // F[i]  (+INF neu i==0)
        long long Sj1 = getS((int)j + 1); // S[j+1] (-INF neu vuot mang)
        long long Sj  = getS((int)j);  // S[j]  (+INF neu j==0)
        long long Fi1 = getF(i + 1);   // F[i+1] (-INF neu vuot mang)

        bool A = Fi > Sj1;  // khong lay qua nhieu tu F
        bool B = Sj > Fi1;  // da lay du tu F

        if (A && B) {
            ans = min(Fi, Sj);
            break;
        } else if (!A) {
            hi = i - 1;     // F[i] qua nho -> lay it hon tu F
        } else {            // !B
            lo = i + 1;     // can lay them tu F
        }
    }

    cout << "! " << ans << endl; // flush
    return 0;
}
