#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    // Buffer đầu ra để in nhanh
    string out;
    out.reserve((size_t)m * 12);
    char buf[32];

    for (int i = 0; i < m; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        // Thứ tự chỉ số 1 < 2 < ... < n là một topological order cố định:
        // luôn định hướng cạnh từ đỉnh chỉ số nhỏ -> đỉnh chỉ số lớn nên
        // đồ thị thu được chắc chắn là DAG
        if (a > b) swap(a, b);
        int len = sprintf(buf, "%d %d\n", a, b);
        out.append(buf, len);
    }

    fputs(out.c_str(), stdout);
    return 0;
}
