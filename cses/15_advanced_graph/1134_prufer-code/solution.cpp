// Prufer Code - CSES 1134
// https://cses.fi/problemset/task/1134
//
// Giải mã Prüfer code để dựng lại cây có n đỉnh.
// deg[v] = số lần v xuất hiện trong code + 1.
// Dùng min-heap chứa các lá (deg == 1): mỗi bước nối lá nhỏ nhất với
// phần tử hiện tại của code, giảm bậc, đẩy lá mới vào heap.
// Cuối cùng nối 2 đỉnh còn lại có deg == 1.

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int m = n - 2;                 // độ dài Prüfer code
    vector<int> code(m);
    vector<int> deg(n + 1, 1);     // deg[v] khởi tạo = 1

    for (int i = 0; i < m; i++) {
        scanf("%d", &code[i]);
        deg[code[i]]++;
    }

    // min-heap các đỉnh lá (deg == 1)
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int v = 1; v <= n; v++)
        if (deg[v] == 1) pq.push(v);

    // Buffer output cho nhanh (in ra nhanh hơn)
    string out;
    out.reserve((size_t)(n) * 12);
    char tmp[32];

    auto emit = [&](int a, int b) {
        int len = snprintf(tmp, sizeof(tmp), "%d %d\n", a, b);
        out.append(tmp, len);
    };

    for (int i = 0; i < m; i++) {
        int x = code[i];
        int leaf = pq.top();
        pq.pop();
        emit(leaf, x);
        if (--deg[x] == 1) pq.push(x);
    }

    // hai đỉnh còn lại
    int a = pq.top(); pq.pop();
    int b = pq.top(); pq.pop();
    emit(a, b);

    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
