#include <bits/stdc++.h>
using namespace std;

// x_i <= 1e9 < 2^30 nên chỉ cần xét các bit 29..0
static const int BITS = 30;

// trie[node][b] = con của node theo bit b (0/1); giá trị 0 nghĩa là chưa có
static int trie[6200005][2];
int trieCnt = 1; // node 0 là gốc

// chèn một prefix XOR vào trie, đi từ bit cao xuống bit thấp
void insertVal(unsigned int x) {
    int cur = 0;
    for (int b = BITS - 1; b >= 0; --b) {
        int bit = (x >> b) & 1u;
        if (!trie[cur][bit]) {
            trie[cur][bit] = trieCnt++;
        }
        cur = trie[cur][bit];
    }
}

// trả về giá trị XOR lớn nhất giữa x và một giá trị đã chèn trong trie
unsigned int queryMax(unsigned int x) {
    int cur = 0;
    unsigned int res = 0;
    for (int b = BITS - 1; b >= 0; --b) {
        int bit = (x >> b) & 1u;
        int want = bit ^ 1; // ưu tiên nhánh ngược để bit kết quả thành 1
        if (trie[cur][want]) {
            res |= (1u << b);
            cur = trie[cur][want];
        } else {
            cur = trie[cur][bit]; // không có thì đi theo nhánh cùng bit
        }
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    unsigned int prefix = 0;
    insertVal(prefix); // chèn p[0] = 0 trước
    unsigned int best = 0;

    // duyệt từng phần tử, cập nhật prefix XOR rồi truy vấn giá trị lớn nhất
    for (int i = 0; i < n; ++i) {
        unsigned int a;
        cin >> a;
        prefix ^= a; // p[i] = p[i-1] ^ a[i]
        best = max(best, queryMax(prefix));
        insertVal(prefix);
    }

    cout << best << '\n';
    return 0;
}
