// Course Schedule II - CSES 1757
// https://cses.fi/problemset/task/1757
//
// Tiêu chí: hoàn thành khóa 1 sớm nhất có thể, rồi khóa 2 sớm nhất có thể, ...
// (tối thiểu hóa vị trí của đỉnh 1, rồi đỉnh 2, ...). KHÁC với topo order nhỏ
// nhất theo từ điển. Cách làm: đảo cạnh, chạy Kahn với MAX-heap, rồi đảo kết quả.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<vector<int>> radj(n + 1);   // đồ thị đảo: cạnh gốc a->b => radj[b] có a
    vector<int> indeg(n + 1, 0);       // in-degree trong đồ thị đảo = out-degree gốc

    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);        // a phải trước b
        radj[b].push_back(a);          // cạnh đảo b -> a
        indeg[a]++;
    }

    // Max-heap: mỗi bước lấy sink có nhãn lớn nhất đặt vào cuối chuỗi.
    priority_queue<int> pq;
    for (int v = 1; v <= n; v++)
        if (indeg[v] == 0) pq.push(v);

    vector<int> order;                 // thứ tự dựng từ CUỐI về ĐẦU
    order.reserve(n);
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        order.push_back(u);
        for (int w : radj[u])
            if (--indeg[w] == 0) pq.push(w);
    }

    // Đề bảo đảm luôn có thứ tự hợp lệ; check này chỉ để phòng thủ, không kích hoạt.
    if ((int)order.size() < n) {
        printf("IMPOSSIBLE\n");
        return 0;
    }

    // Đảo ngược "order" để có thứ tự hoàn thành cần in.
    for (int i = n - 1; i >= 0; i--) {
        printf("%d", order[i]);
        putchar(i ? ' ' : '\n');
    }
    return 0;
}
