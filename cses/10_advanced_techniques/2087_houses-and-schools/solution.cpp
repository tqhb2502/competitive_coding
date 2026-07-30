#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int n, k;
int N1;
vector<ll> pc, pw;   // tiền tố của c[] và của i*c[i]
vector<ll> C;        // bảng chi phí: C[r*N1 + l] = cost([l,r])
vector<ll> dpPrev, dpCur;
const ll INF = (ll)4e18;

// Divide & Conquer DP optimization cho một tầng: tính dpCur[i] với i thuộc [lo,hi],
// biết điểm tối ưu p nằm trong [optlo, opthi]. Nhờ điều kiện Monge nên opt đơn điệu.
void solve(int lo, int hi, int optlo, int opthi){
    if(lo > hi) return;
    int mid = (lo + hi) >> 1;
    ll best = INF;
    int bestp = optlo;
    int uphi = min(opthi, mid - 1);
    ll rowBase = (ll)mid * N1;
    // Duyệt các điểm cắt p khả dĩ để tìm dpCur[mid] và vị trí tối ưu bestp
    for(int p = optlo; p <= uphi; p++){
        if(dpPrev[p] >= INF) continue;
        ll val = dpPrev[p] + C[rowBase + (p + 1)];
        if(val < best){ best = val; bestp = p; }
    }
    dpCur[mid] = best;
    // Đệ quy: nửa trái dùng opt trong [optlo, bestp], nửa phải trong [bestp, opthi]
    solve(lo, mid - 1, optlo, bestp);
    solve(mid + 1, hi, bestp, opthi);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> k;
    // Xây tiền tố: pc là tổng số trẻ, pw là tổng i*c[i] để tính chi phí L1
    pc.assign(n + 1, 0);
    pw.assign(n + 1, 0);
    for(int i = 1; i <= n; i++){
        ll ci; cin >> ci;
        pc[i] = pc[i-1] + ci;
        pw[i] = pw[i-1] + (ll)i * ci;
    }

    N1 = n + 1;
    C.assign((ll)N1 * N1, 0);

    // Tính trước bảng cost cho mọi đoạn [l,r]. Cố định r, cho l giảm dần từ r về 1;
    // weighted median m (chỉ số nhỏ nhất sao cho 2*(pc[m]-pc[l-1]) >= tổng trọng số)
    // di chuyển đơn điệu về trái khi l giảm -> dùng two-pointer, O(n) mỗi r.
    for(int r = 1; r <= n; r++){
        int m = r;
        ll rowBase = (ll)r * N1;
        for(int l = r; l >= 1; l--){
            ll total = pc[r] - pc[l-1];
            while(m - 1 >= l && 2*(pc[m-1] - pc[l-1]) >= total) m--;
            // Chi phí đoạn = phần bên trái median + phần bên phải median
            ll cost = (ll)m * (pc[m] - pc[l-1]) - (pw[m] - pw[l-1])
                    + (pw[r] - pw[m]) - (ll)m * (pc[r] - pc[m]);
            C[rowBase + l] = cost;
        }
    }

    // Quy hoạch động theo số đoạn: dpPrev là tầng j-1, dpCur là tầng j
    dpPrev.assign(n + 1, INF);
    dpCur.assign(n + 1, INF);
    dpPrev[0] = 0;

    for(int j = 1; j <= k; j++){
        fill(dpCur.begin(), dpCur.end(), INF);
        // i chạy từ j đến n; điểm cắt p chạy từ j-1 đến i-1 (tối đa n-1)
        solve(j, n, j - 1, n - 1);
        swap(dpPrev, dpCur);
    }

    cout << dpPrev[n] << "\n";
    return 0;
}
