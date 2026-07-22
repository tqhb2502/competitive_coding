// Acyclic Graph Edges - CSES 1756
// https://cses.fi/problemset/task/1756
//
// Định hướng mỗi cạnh vô hướng từ đỉnh chỉ số nhỏ hơn sang đỉnh chỉ số lớn hơn.
// Thứ tự các chỉ số 1 < 2 < ... < n chính là một topological order, nên đồ thị
// có hướng thu được luôn là DAG (mọi cạnh đi theo chiều tăng chỉ số, không thể
// tạo chu trình). Special judge chấp nhận mọi lời giải hợp lệ.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    // Buffer đầu ra để in nhanh.
    string out;
    out.reserve((size_t)m * 12);
    char buf[32];

    for (int i = 0; i < m; ++i) {
        int a, b;
        scanf("%d %d", &a, &b);
        if (a > b) swap(a, b);           // luôn hướng từ chỉ số nhỏ -> chỉ số lớn
        int len = sprintf(buf, "%d %d\n", a, b);
        out.append(buf, len);
    }

    fputs(out.c_str(), stdout);
    return 0;
}
