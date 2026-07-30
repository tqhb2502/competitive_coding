#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    // Ma trận chi phí 1-indexed: a[i][j] = chi phí gán công việc j cho nhân viên i.
    vector<vector<long long>> a(n + 1, vector<long long>(n + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            cin >> a[i][j];

    const long long INF = LLONG_MAX / 4;

    // Thuật toán Hungarian (Kuhn-Munkres).
    // u[i], v[j] là hệ thế năng; p[j] = nhân viên đang gán cho công việc j.
    vector<long long> u(n + 1, 0), v(n + 1, 0);
    vector<int> p(n + 1, 0), way(n + 1, 0);

    // Thêm lần lượt từng hàng i vào matching.
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        // minv[j]: khoảng cách reduced cost nhỏ nhất tới cột j; way[j]: cột tiền nhiệm.
        vector<long long> minv(n + 1, INF);
        vector<char> used(n + 1, false);
        // Bước kiểu Dijkstra trên reduced cost tìm đường tăng.
        do {
            used[j0] = true;
            int i0 = p[j0], j1 = -1;
            long long delta = INF;
            for (int j = 1; j <= n; ++j) {
                if (!used[j]) {
                    long long cur = a[i0][j] - u[i0] - v[j];
                    if (cur < minv[j]) { minv[j] = cur; way[j] = j0; }
                    if (minv[j] < delta) { delta = minv[j]; j1 = j; }
                }
            }
            // Cập nhật thế năng u, v theo delta để giữ bất biến reduced cost >= 0.
            for (int j = 0; j <= n; ++j) {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                else { minv[j] -= delta; }
            }
            j0 = j1;
        } while (p[j0] != 0);   // dừng khi chạm một cột chưa được gán
        // Lật ngược đường tăng vừa tìm để cập nhật matching p[].
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }

    // p[j] = nhân viên gán cho công việc j; đảo lại thành công việc theo nhân viên.
    vector<int> assignTask(n + 1, 0);
    long long total = 0;
    for (int j = 1; j <= n; ++j) {
        assignTask[p[j]] = j;
        total += a[p[j]][j];
    }

    cout << total << '\n';
    for (int i = 1; i <= n; ++i)
        cout << i << ' ' << assignTask[i] << '\n';

    return 0;
}
