#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x;
    cin >> n >> x;

    vector<long long> s(n + 1), f(n + 1);
    for (int i = 1; i <= n; i++) cin >> s[i];
    for (int i = 1; i <= n; i++) cin >> f[i];

    // Lower envelope: mỗi đường là y = M[k]*X + B[k], hệ số góc M giảm thực sự.
    vector<long long> M, B;

    // Kiểm tra đường l2 có thừa hay không khi kẹp giữa l1 (trái) và l3 (phải).
    // Vì hệ số góc giảm thực sự nên (M[l1]-M[l2]) > 0 và (M[l1]-M[l3]) > 0.
    // Đường l2 thừa khi giao_x(l1,l3) <= giao_x(l1,l2). Dùng __int128 tránh tràn.
    auto bad = [&](int l1, int l2, int l3) -> bool {
        return (__int128)(B[l3] - B[l1]) * (M[l1] - M[l2])
             <= (__int128)(B[l2] - B[l1]) * (M[l1] - M[l3]);
    };

    // Thêm một đường thẳng vào cuối envelope, giữ tính lồi dưới.
    auto add = [&](long long m, long long b) {
        int sz = (int)M.size();
        // Nếu trùng hệ số góc với đường cuối, chỉ giữ đường có intercept nhỏ hơn.
        if (sz > 0 && M[sz - 1] == m) {
            if (B[sz - 1] <= b) return;      // đường cũ đã dominate đường mới
            M.pop_back();
            B.pop_back();
        }
        M.push_back(m);
        B.push_back(b);
        // Loại các đường ở giữa đã trở nên thừa.
        while ((int)M.size() >= 3) {
            int t = (int)M.size();
            if (bad(t - 3, t - 2, t - 1)) {
                M[t - 2] = M[t - 1];
                B[t - 2] = B[t - 1];
                M.pop_back();
                B.pop_back();
            } else break;
        }
    };

    // Truy vấn min tại X = q bằng binary search (giá trị theo chỉ số là unimodal).
    auto query = [&](long long q) -> long long {
        int lo = 0, hi = (int)M.size() - 1;
        while (lo < hi) {
            int mid = (lo + hi) / 2;
            if (M[mid] * q + B[mid] > M[mid + 1] * q + B[mid + 1]) lo = mid + 1;
            else hi = mid;
        }
        return M[lo] * q + B[lo];
    };

    // Trạng thái ban đầu: đường hệ số góc x, intercept dp[0] = 0.
    add(x, 0);

    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        // dp[i] = min các đường tại X = s[i].
        long long dp = query(s[i]);
        if (i == n) ans = dp;      // bắt buộc giết con quái cuối cùng
        // Thêm đường ứng với việc đã giết xong con quái i (hệ số góc f[i]).
        add(f[i], dp);
    }

    cout << ans << "\n";
    return 0;
}
