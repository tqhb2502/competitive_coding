#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 0;

    vector<vector<int>> radj(n + 1);   // đồ thị đảo: cạnh gốc a->b => radj[b] chứa a
    vector<int> indeg(n + 1, 0);       // in-degree trong đồ thị đảo = out-degree gốc

    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);        // ràng buộc: khóa a phải hoàn thành trước khóa b
        radj[b].push_back(a);          // thêm cạnh đảo b -> a
        indeg[a]++;
    }

    // Max-heap: mỗi bước lấy sink (đỉnh có indeg = 0 trong đồ thị đảo) có nhãn
    // lớn nhất, đặt vào vị trí cuối cùng còn trống của chuỗi.
    priority_queue<int> pq;
    for (int v = 1; v <= n; v++)
        if (indeg[v] == 0) pq.push(v);

    // Kahn trên đồ thị đảo: dựng "order" theo thứ tự từ CUỐI về ĐẦU.
    vector<int> order;
    order.reserve(n);
    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        order.push_back(u);
        for (int w : radj[u])          // giảm indeg các đỉnh kề; về 0 thì đẩy vào heap
            if (--indeg[w] == 0) pq.push(w);
    }

    // Đề bảo đảm luôn có thứ tự hợp lệ; kiểm tra này chỉ để phòng thủ, không kích hoạt.
    if ((int)order.size() < n) {
        printf("IMPOSSIBLE\n");
        return 0;
    }

    // Đảo ngược "order" để ra thứ tự hoàn thành cần in.
    for (int i = n - 1; i >= 0; i--) {
        printf("%d", order[i]);
        putchar(i ? ' ' : '\n');
    }
    return 0;
}
