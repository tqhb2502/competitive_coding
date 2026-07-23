#include <bits/stdc++.h>
using namespace std;

// DSU (Union-Find) với union by size + path halving
static int par[100005];
static int sz[100005];

int findp(int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];   // nén đường đi (path halving)
        x = par[x];
    }
    return x;
}

int main() {
    int n, m, k;
    if (scanf("%d %d %d", &n, &m, &k) != 3) return 0;

    // Đọc m cạnh và định danh mỗi cạnh bằng khóa (min, max) -> chỉ số cạnh
    vector<int> ea(m), eb(m);
    unordered_map<long long, int> emap;
    emap.reserve(m * 2);
    emap.max_load_factor(0.7f);

    for (int i = 0; i < m; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        ea[i] = a;
        eb[i] = b;
        long long lo = min(a, b), hi = max(a, b);
        long long key = lo * 100001LL + hi;   // n <= 1e5 => an toàn, không đụng độ
        emap[key] = i;
    }

    // Đánh dấu k cạnh bị hỏng và lưu thứ tự chúng bị hỏng
    vector<char> broken(m, 0);
    vector<int> ord(k);   // ord[i] = chỉ số cạnh bị hỏng ở lần hỏng thứ (i+1)
    for (int i = 0; i < k; i++) {
        int a, b;
        scanf("%d %d", &a, &b);
        long long lo = min(a, b), hi = max(a, b);
        long long key = lo * 100001LL + hi;
        int idx = emap[key];
        ord[i] = idx;
        broken[idx] = 1;
    }

    // Khởi tạo DSU: mỗi đỉnh là một thành phần liên thông riêng
    for (int i = 1; i <= n; i++) { par[i] = i; sz[i] = 1; }
    int comps = n;

    // Union các cạnh KHÔNG bao giờ bị hỏng (base graph = trạng thái sau lần hỏng k)
    for (int i = 0; i < m; i++) {
        if (broken[i]) continue;
        int ra = findp(ea[i]), rb = findp(eb[i]);
        if (ra != rb) {
            if (sz[ra] < sz[rb]) swap(ra, rb);
            par[rb] = ra;
            sz[ra] += sz[rb];
            comps--;   // hai thành phần hợp nhất -> số thành phần giảm 1
        }
    }

    vector<int> result(k);
    result[k - 1] = comps;   // số thành phần liên thông sau lần hỏng thứ k

    // Xử lý NGƯỢC THỜI GIAN: thêm lại cạnh bị hỏng ở lần k, k-1, ..., 2
    // Thêm cạnh hỏng ở lần i -> khôi phục trạng thái sau lần hỏng thứ (i-1)
    for (int i = k - 1; i >= 1; i--) {
        int idx = ord[i];
        int ra = findp(ea[idx]), rb = findp(eb[idx]);
        if (ra != rb) {
            if (sz[ra] < sz[rb]) swap(ra, rb);
            par[rb] = ra;
            sz[ra] += sz[rb];
            comps--;
        }
        result[i - 1] = comps;   // trạng thái sau lần hỏng thứ (i-1)
    }

    // In k kết quả trên một dòng, cách nhau bởi dấu cách
    for (int i = 0; i < k; i++) {
        printf("%d", result[i]);
        if (i + 1 < k) printf(" ");
    }
    printf("\n");
    return 0;
}
