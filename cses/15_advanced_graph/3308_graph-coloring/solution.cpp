#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    // adj[v] = bitmask các đỉnh kề với đỉnh v
    vector<int> adj(n, 0);
    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        --a; --b;
        if (a != b) {
            adj[a] |= (1 << b);
            adj[b] |= (1 << a);
        }
    }

    int full = (1 << n) - 1;

    // indep[S] = true nếu S là independent set (không có cạnh nằm trong S).
    // Tính tăng dần: tách bit thấp nhất v, S độc lập khi phần còn lại độc lập
    // và v không kề đỉnh nào khác trong S.
    vector<char> indep(1 << n, 0);
    indep[0] = 1;
    for (int S = 1; S <= full; S++) {
        int v = __builtin_ctz(S);          // đỉnh (bit) thấp nhất của S
        indep[S] = indep[S ^ (1 << v)] && ((adj[v] & S) == 0);
    }

    // dp[S] = số màu ít nhất để tô đúng tập đỉnh S.
    // choice[S] = lớp màu (independent set) được chọn để truy vết.
    const int INF = 1e9;
    vector<int> dp(1 << n, INF), choice(1 << n, 0);
    dp[0] = 0;
    for (int S = 1; S <= full; S++) {
        int low = S & (-S);                // giữ lại bit thấp nhất của S
        int rest = S ^ low;
        // duyệt mọi tập con của S có chứa 'low' làm lớp màu ứng viên T;
        // đỉnh low luôn phải thuộc một lớp màu nên chỉ cần xét các T chứa nó
        for (int sub = rest; ; sub = (sub - 1) & rest) {
            int T = sub | low;             // lớp màu ứng viên (chứa low)
            if (indep[T] && dp[S ^ T] + 1 < dp[S]) {
                dp[S] = dp[S ^ T] + 1;
                choice[S] = T;
            }
            if (sub == 0) break;
        }
    }

    // Truy vết cách tô: mỗi lớp màu choice[S] được gán một màu mới
    vector<int> color(n, 0);
    int S = full, c = 0;
    while (S) {
        int T = choice[S];
        ++c;
        for (int v = 0; v < n; v++)
            if (T & (1 << v)) color[v] = c;
        S ^= T;
    }

    printf("%d\n", dp[full]);
    for (int i = 0; i < n; i++)
        printf("%d%c", color[i], i + 1 < n ? ' ' : '\n');
    return 0;
}
