// Houses and Schools - CSES 2087
// https://cses.fi/problemset/task/2087
//
// Ta co n nha tren mot duong thang (vi tri 1..n), nha i co c[i] tre em.
// Dat k truong hoc tai k trong so cac nha; moi tre di den truong gan nhat,
// khoang cach = hieu vi tri. Toi thieu hoa tong khoang cach.
//
// Loi giai toi uu chia n nha thanh k doan lien tiep; moi doan dat truong tai
// weighted median cua doan -> chi phi = tong c[i]*|i - median|.
//
// dp[i][j] = chi phi nho nhat khi phuc vu i nha dau bang j truong (j doan).
// dp[i][j] = min_{p<i} dp[p][j-1] + cost(p+1, i).
// Ham cost(l,r) thoa quadrangle inequality (Monge) => diem toi uu (opt) don dieu
// => moi tang j tinh bang Divide & Conquer DP optimization: O(n log n) truy van/tang.
//
// De cost(l,r) la O(1), ta tinh truoc bang C[r][l] bang two-pointer median: O(n^2).
// Tong: O(n^2 + n*k*log n).

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int n, k;
int N1;
vector<ll> pc, pw;   // prefix sum cua c va cua i*c
vector<ll> C;        // bang chi phi, C[r*N1 + l] = cost([l,r])
vector<ll> dpPrev, dpCur;
const ll INF = (ll)4e18;

// D&C DP optimization cho mot tang: tinh dpCur[i] cho i in [lo,hi],
// biet diem toi uu p nam trong [optlo, opthi].
void solve(int lo, int hi, int optlo, int opthi){
    if(lo > hi) return;
    int mid = (lo + hi) >> 1;
    ll best = INF;
    int bestp = optlo;
    int uphi = min(opthi, mid - 1);
    ll rowBase = (ll)mid * N1;
    for(int p = optlo; p <= uphi; p++){
        if(dpPrev[p] >= INF) continue;
        ll val = dpPrev[p] + C[rowBase + (p + 1)];
        if(val < best){ best = val; bestp = p; }
    }
    dpCur[mid] = best;
    solve(lo, mid - 1, optlo, bestp);
    solve(mid + 1, hi, bestp, opthi);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> k;
    pc.assign(n + 1, 0);
    pw.assign(n + 1, 0);
    for(int i = 1; i <= n; i++){
        ll ci; cin >> ci;
        pc[i] = pc[i-1] + ci;
        pw[i] = pw[i-1] + (ll)i * ci;
    }

    N1 = n + 1;
    C.assign((ll)N1 * N1, 0);

    // Tinh truoc bang cost. Voi moi r co dinh, l giam dan tu r ve 1;
    // weighted median m (chi so nho nhat sao cho 2*(pc[m]-pc[l-1]) >= tong)
    // di chuyen don dieu ve trai khi l giam -> two-pointer O(n) moi r.
    for(int r = 1; r <= n; r++){
        int m = r;
        ll rowBase = (ll)r * N1;
        for(int l = r; l >= 1; l--){
            ll total = pc[r] - pc[l-1];
            while(m - 1 >= l && 2*(pc[m-1] - pc[l-1]) >= total) m--;
            ll cost = (ll)m * (pc[m] - pc[l-1]) - (pw[m] - pw[l-1])
                    + (pw[r] - pw[m]) - (ll)m * (pc[r] - pc[m]);
            C[rowBase + l] = cost;
        }
    }

    dpPrev.assign(n + 1, INF);
    dpCur.assign(n + 1, INF);
    dpPrev[0] = 0;

    for(int j = 1; j <= k; j++){
        fill(dpCur.begin(), dpCur.end(), INF);
        // i chay tu j den n; p chay tu j-1 den i-1 (toi da n-1)
        solve(j, n, j - 1, n - 1);
        swap(dpPrev, dpCur);
    }

    cout << dpPrev[n] << "\n";
    return 0;
}
