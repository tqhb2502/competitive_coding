// Task Assignment - https://cses.fi/problemset/task/2129
// Minimum-cost perfect matching (assignment problem) solved with the
// Hungarian algorithm (Kuhn-Munkres) in O(n^3). Fast for n <= 200.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    // 1-indexed cost matrix: a[i][j] = cost of assigning task j to employee i.
    vector<vector<long long>> a(n + 1, vector<long long>(n + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            cin >> a[i][j];

    const long long INF = LLONG_MAX / 4;

    // Hungarian algorithm (e-maxx style).
    // u[i], v[j] are potentials; p[j] = employee matched to task j.
    vector<long long> u(n + 1, 0), v(n + 1, 0);
    vector<int> p(n + 1, 0), way(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<long long> minv(n + 1, INF);
        vector<char> used(n + 1, false);
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
            for (int j = 0; j <= n; ++j) {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                else { minv[j] -= delta; }
            }
            j0 = j1;
        } while (p[j0] != 0);
        // Flip the matching along the found augmenting path.
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }

    // p[j] = employee assigned to task j; invert to task-per-employee.
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
