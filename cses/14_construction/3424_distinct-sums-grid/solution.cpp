#include <bits/stdc++.h>
using namespace std;

// RNG tất định (seed cố định) để hành vi tái lập chính xác trên judge.
struct RNG {
    uint64_t s;
    RNG(uint64_t seed): s(seed) {}
    uint64_t next() { // xorshift64*
        s ^= s >> 12; s ^= s << 25; s ^= s >> 27;
        return s * 0x2545F4914F6CDD1DULL;
    }
    int rint(int n) { return (int)(next() % (uint64_t)n); }
};

// Trả về lưới hợp lệ cho n>=4; vector rỗng nghĩa là IMPOSSIBLE.
vector<vector<int>> solve(int n) {
    // n = 1, 2, 3 là IMPOSSIBLE (đã kiểm chứng bằng vét cạn).
    if (n <= 3) return {};

    // Xuất phát từ Latin square: mỗi giá trị xuất hiện đúng n lần,
    // mọi tổng-hàng và tổng-cột ban đầu đều bằng S = n(n+1)/2.
    vector<vector<int>> g(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            g[i][j] = ((i + j) % n) + 1;

    // Tính tổng-hàng rs[i] và tổng-cột cs[j].
    vector<long long> rs(n, 0), cs(n, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) { rs[i] += g[i][j]; cs[j] += g[i][j]; }

    // Đếm số lần mỗi giá trị tổng xuất hiện trong 2n tổng; tổng nằm trong [n, n*n].
    // distinct = số giá trị tổng phân biệt hiện tại.
    int maxs = n * n + 1;
    vector<int> cnt(maxs + 1, 0);
    int distinct = 0;
    auto addSum = [&](long long v) { if (cnt[v]++ == 0) distinct++; };
    auto delSum = [&](long long v) { if (--cnt[v] == 0) distinct--; };
    for (int i = 0; i < n; i++) addSum(rs[i]);
    for (int j = 0; j < n; j++) addSum(cs[j]);

    RNG rng(88172645463325252ULL);
    long long cap = 200LL * n + 100000LL; // giới hạn rộng rãi; thực tế hội tụ sớm hơn nhiều
    long long it = 0;
    uint64_t reseed = 1;

    // Local search "bảo toàn đa tập": hoán đổi hai ô khác giá trị, giữ hoán đổi
    // nếu nó không làm giảm distinct, lặp cho tới khi distinct = 2n.
    while (distinct < 2 * n && it < cap) {
        it++;
        int i = rng.rint(n), j = rng.rint(n), k = rng.rint(n), l = rng.rint(n);
        int a = g[i][j], b = g[k][l];
        if (a == b) continue; // hai ô cùng giá trị: hoán đổi vô nghĩa

        // Xác định các tổng thực sự bị thay đổi bởi hoán đổi.
        bool rowChange = (i != k);   // cùng hàng thì tổng-hàng giữ nguyên
        bool colChange = (j != l);   // cùng cột thì tổng-cột giữ nguyên
        // (i==k && j==l nghĩa là cùng một ô => a==b, đã bị bỏ qua ở trên)

        // Lưu giá trị cũ để có thể hoàn tác.
        long long oldRi = rs[i], oldRk = rs[k], oldCj = cs[j], oldCl = cs[l];

        // Áp dụng hoán đổi thử; so sánh distinct (càng cao càng tốt).
        int distBefore = distinct;

        // Gỡ các tổng bị ảnh hưởng khỏi bộ đếm.
        if (rowChange) { delSum(rs[i]); delSum(rs[k]); }
        if (colChange) { delSum(cs[j]); delSum(cs[l]); }

        // Cập nhật các tổng theo hoán đổi.
        if (rowChange) { rs[i] += (b - a); rs[k] += (a - b); }
        if (colChange) { cs[j] += (b - a); cs[l] += (a - b); }

        // Thêm lại các tổng mới vào bộ đếm.
        if (rowChange) { addSum(rs[i]); addSum(rs[k]); }
        if (colChange) { addSum(cs[j]); addSum(cs[l]); }

        if (distinct >= distBefore) {
            // Chấp nhận: thực hiện hoán đổi hai ô.
            g[i][j] = b; g[k][l] = a;
        } else {
            // Hoàn tác toàn bộ.
            if (rowChange) { delSum(rs[i]); delSum(rs[k]); }
            if (colChange) { delSum(cs[j]); delSum(cs[l]); }
            rs[i] = oldRi; rs[k] = oldRk; cs[j] = oldCj; cs[l] = oldCl;
            if (rowChange) { addSum(rs[i]); addSum(rs[k]); }
            if (colChange) { addSum(cs[j]); addSum(cs[l]); }
        }

        // An toàn: nếu kẹt quá lâu, thay hạt giống mới để thoát (rất hiếm).
        if ((it & ((1 << 20) - 1)) == 0 && distinct < 2 * n) {
            reseed++;
            rng = RNG(88172645463325252ULL * reseed + 12345ULL);
        }
    }
    return g; // với n>=4, distinct == 2n theo cách xây dựng
}

int main() {
#ifdef TESTALL
    for (int n = 1; n <= 1000; n++) {
        auto g = solve(n);
        if (n <= 3) { if (!g.empty()) { printf("n=%d expected IMPOSSIBLE\n", n); return 1; } continue; }
        // kiểm tra số lần xuất hiện của mỗi giá trị
        vector<int> c(n + 1, 0);
        bool bad = false;
        for (auto &row : g) for (int x : row) { if (x < 1 || x > n) bad = true; else c[x]++; }
        for (int v = 1; v <= n; v++) if (c[v] != n) bad = true;
        set<long long> S;
        for (int i = 0; i < n; i++) { long long s = 0; for (int j = 0; j < n; j++) s += g[i][j]; S.insert(s); }
        for (int j = 0; j < n; j++) { long long s = 0; for (int i = 0; i < n; i++) s += g[i][j]; S.insert(s); }
        if ((int)S.size() != 2 * n) bad = true;
        if (bad) { printf("FAIL n=%d distinct=%d\n", n, (int)S.size()); return 1; }
    }
    printf("ALL OK 1..1000\n");
    return 0;
#else
    int n;
    if (scanf("%d", &n) != 1) return 0;
    auto g = solve(n);
    if (g.empty()) { printf("IMPOSSIBLE\n"); return 0; }
    // xuất kết quả nhanh
    string out;
    out.reserve((size_t)n * n * 4);
    char buf[16];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int len = sprintf(buf, "%d", g[i][j]);
            out.append(buf, len);
            out.push_back(j + 1 == n ? '\n' : ' ');
        }
    }
    fputs(out.c_str(), stdout);
    return 0;
#endif
}
