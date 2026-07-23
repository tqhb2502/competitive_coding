#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int m = n - 2;                 // độ dài của Prüfer code
    vector<int> code(m);
    vector<int> deg(n + 1, 1);     // khởi tạo bậc deg[v] = 1 cho mọi đỉnh

    // Đỉnh xuất hiện k lần trong code sẽ có bậc = k + 1
    for (int i = 0; i < m; i++) {
        scanf("%d", &code[i]);
        deg[code[i]]++;
    }

    // min-heap chứa các đỉnh đang là lá (deg == 1)
    priority_queue<int, vector<int>, greater<int>> pq;
    for (int v = 1; v <= n; v++)
        if (deg[v] == 1) pq.push(v);

    // Buffer output để in ra nhanh hơn
    string out;
    out.reserve((size_t)(n) * 12);
    char tmp[32];

    auto emit = [&](int a, int b) {
        int len = snprintf(tmp, sizeof(tmp), "%d %d\n", a, b);
        out.append(tmp, len);
    };

    // Mỗi bước: nối lá nhỏ nhất với phần tử hiện tại của code,
    // giảm bậc của phần tử đó; nếu nó thành lá thì đẩy vào heap
    for (int i = 0; i < m; i++) {
        int x = code[i];
        int leaf = pq.top();
        pq.pop();
        emit(leaf, x);
        if (--deg[x] == 1) pq.push(x);
    }

    // Nối hai đỉnh còn lại có deg == 1 thành cạnh cuối cùng
    int a = pq.top(); pq.pop();
    int b = pq.top(); pq.pop();
    emit(a, b);

    fwrite(out.data(), 1, out.size(), stdout);
    return 0;
}
